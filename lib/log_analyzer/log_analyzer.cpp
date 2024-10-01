#include <iostream>
#include <cstring>

#include "log_analyzer.hpp"
#include "../utils/print_util.hpp" 
#include "../utils/time_util.hpp"


void LogAnalyzer::SetUp(int argc, char** argv) {
    line_ = new char[lineLenInit];

    err_code_ = args_parser_
        .AddFlag("help", 'h')
        .AddFlag("print", 'p')
        .AddOption("output", 'o')
        .AddOption("stats", 's')
        .AddOption("window", 'w')
        .AddOption("from", 'f')
        .AddOption("to", 'e')
        .Parse(argc, argv);

    if (err_code_ != 0) {
        return;
    }

    SetUpHelp();
    SetUpInput();
    SetUpOutput();
    SetUpPrint();
    SetUpStats();
    SetUpWindow();
    SetUpFrom();
    SetUpTo();
}

void LogAnalyzer::SetUpHelp() {
    if (args_parser_.GetFlag("help").value().declared) {
        PrintUtil::PrintlnInfo(USAGE_TEXT);
    }
}

void LogAnalyzer::SetUpInput() {
    auto free_args_size = args_parser_.GetFreeArgsSize();
    if (free_args_size == 0) {
        PrintUtil::PrintlnCrit("The input file is missing, nothing to do.");
        err_code_ = 1;

        return;
    }

    has_input_path_ = true;
    if (free_args_size > 1) {
        PrintUtil::PrintlnWarn("Multiple input files have been given, the last one will be selected.");
    }
    
    auto input_file_name = args_parser_.GetFreeArgs()[free_args_size - 1];
    input_stream_ = new std::ifstream(input_file_name, std::ios::binary);
    if (!input_stream_->is_open()) {
        PrintUtil::PrintlnCrit("Input file doesn't exist: ");
        PrintUtil::PrintlnCrit(input_file_name);
        err_code_ = 1;
    }
}

void LogAnalyzer::SetUpOutput() {
    auto opt = args_parser_.GetOption("output").value();
    has_output_path_ = opt.declared && opt.value_declared;
    if (has_output_path_) {
        output_stream_ = new std::ofstream(opt.value, std::ios::binary);

        if (!output_stream_->is_open()) {
            has_output_path_ = false;
            PrintUtil::PrintlnWarn("Can't open output file: ");
            PrintUtil::PrintlnWarn(opt.value);
            PrintUtil::PrintlnWarn("Output option was disabled.");
        }
    }
}

void LogAnalyzer::SetUpPrint() {
    print_error_requests_ = args_parser_.GetFlag("print").value().declared;
}

void LogAnalyzer::SetUpStats() {
    auto opt = args_parser_.GetOption("stats").value();
    print_most_freq_err_requests_ = opt.declared;
    if (opt.value_declared) {
        most_freq_err_requests_len_ = std::atoi(opt.value);
    }
}

void LogAnalyzer::SetUpWindow() {
    auto opt = args_parser_.GetOption("window").value();
    print_max_requests_window_ = opt.declared;
    if (opt.value_declared) {
        max_requests_window_len_seconds_ = std::atoi(opt.value);
    }
}

void LogAnalyzer::SetUpFrom() {
    auto opt = args_parser_.GetOption("from").value();
    has_lower_time_bound_ = opt.declared;
    if (opt.value_declared) {
        lower_time_bound_unix_ = std::atol(opt.value);
    }
}

void LogAnalyzer::SetUpTo() {
    auto opt = args_parser_.GetOption("to").value();
    has_upper_time_bound_ = opt.declared;
    if (opt.value_declared) {
        upper_time_bound_unix_ = std::atol(opt.value);
    }
}


int LogAnalyzer::Run() {
    if (err_code_ != 0) {
        return err_code_;
    }

    while (!ReachedEnd()) {
        requests_total++;
        ReadRequest();
        if (err_code_ != 0) {
            err_code_ = 0;
            ++err_amount_;
            continue;
        }
        if (curr_request_.status_code >= 500) {
            if (print_error_requests_ || has_output_path_) {
                Write(curr_request_.url);
            }
        }
    }
    WriteBuffer();

    if (err_amount_ > 0) {
        PrintUtil::PrintlnWarn("Requests containing errors were detected:");
        PrintUtil::PrintlnWarn(err_amount_);
    }

    PrintUtil::PrintlnInfo("Processed requests:");
    PrintUtil::PrintlnInfo(requests_total);

    return 0;
}

void LogAnalyzer::ReadRequest() {
    ReadNextLine();
    err_code_ = 0;
    line_pos_ = line_;

    // parse remote-addr
    curr_request_.remote_addr = line_pos_;
    FindInLine(' ');
    SplitLine();

    // parse datetime
    FindInLine('[');
    char* datetime_str = line_pos_;
    FindInLine(']');
    IncrementLinePos();
    SplitLine();
    if (err_code_ == 0) {
        curr_request_.datetime_unix = TimeUtil::ConvertLogsTimeToUnix(datetime_str);
    }

    // parse curr_request url
    FindInLine('\"');
    curr_request_.url = line_pos_;
    IncrementLinePos();
    FindInLineFromEnd('\"');
    IncrementLinePos();
    SplitLine();

    // parse status code
    char* status_str = line_pos_;
    FindInLine(' ');
    SplitLine();
    if (status_str == nullptr) {
        err_code_ = 1;
        
    } else if (err_code_ == 0) {
        curr_request_.status_code = static_cast<uint32_t>(std::atoi(status_str));
        if (!(200 <= curr_request_.status_code && curr_request_.status_code <= 599)) {
            err_code_ = 1;
        }
    }
}

char* LogAnalyzer::ReadNextPart() {
    char* part = new char[255];

    char ch;
    do {
        ch = Get();
    } while (IsWhitespace(ch));

    char* pos = part;
    
    while (!IsWhitespace(ch) && ch != EOF) {
        *pos = ch;
        ++pos;
        ch = Get();
    }

    *pos = '\0';

    return part;
}

size_t LogAnalyzer::ReadNextLine() {
    char ch;
    do {
        ch = Get();
    } while (IsWhitespace(ch));

    line_pos_ = nullptr;
    if (line_size_ != lineLenInit) {
        line_ = new char[lineLenInit];
        line_size_ = lineLenInit;
    }
    char* pos = line_;
    char* end_of_line = line_ + line_size_ - 1;
    
    while (ch != '\n' && ch != EOF) {
        *pos = ch;
        if (pos == end_of_line) { // resize
            char* new_line = new char[line_size_ * 2];
            strncpy(new_line, line_, line_size_);
            pos = new_line + line_size_ - 1;
            line_size_ = line_size_ * 2;
            end_of_line = new_line + line_size_ - 1;

            delete line_;
            line_ = new_line;
        }
        ++pos;
        
        ch = Get();
    }

    *pos = '\0';

    line_len_ = pos - line_;

    return line_len_;
}

char LogAnalyzer::Get() {
    if (read_buffer_pos_ < bytes_read_) {
        return read_buffer_[read_buffer_pos_++];
    }
    if (input_stream_->eof()) {
        return EOF;
    }
    input_stream_->read(read_buffer_, readBufferSize);
    bytes_read_ = input_stream_->gcount();
    read_buffer_pos_ = 0;


    return read_buffer_[read_buffer_pos_++];
}

bool LogAnalyzer::ReachedEnd() {
    return read_buffer_pos_ == bytes_read_ && input_stream_->eof();
}

void LogAnalyzer::FindInLine(char target) {
    if (err_code_ == 0) {
        while (*line_pos_ != target) {
            if (*line_pos_ == '\0') {
                err_code_ = 1;
                line_pos_ = nullptr;
                return;
            }

            line_pos_++;
        }
    }
}

void LogAnalyzer::FindInLineFromEnd(char target) {
    if (err_code_ == 0) {
        char* start = line_pos_;
        line_pos_ = line_ + line_len_ - 1;
        while (line_pos_ != start && *line_pos_ != target) {
            if (line_pos_ == start) {
                err_code_ = 1;
                line_pos_ = nullptr;
                return;
            }

            line_pos_--;
        }
    }
}

void LogAnalyzer::SplitLine() {
    if (err_code_ == 0) {
        *line_pos_ = '\0';
        line_pos_++;
        if (*line_pos_ == '\0') {
            err_code_ = 1;
            line_pos_ = nullptr;
        }
    }
}

void LogAnalyzer::IncrementLinePos() {
    if (err_code_ == 0) {
        line_pos_++;
        if (*line_pos_ == '\0') {
            err_code_ = 1;
            line_pos_ = nullptr;
        }
    }
}

bool LogAnalyzer::IsWhitespace(char ch) {
    return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

void LogAnalyzer::Write(char* text) {
    while (*text != '\0') {
        write_buffer_[write_buffer_pos_] = *text;
        ++text;
        ++write_buffer_pos_;
        if (write_buffer_pos_ == writeBufferSize) {
            WriteBuffer();
        }
    }
}

void LogAnalyzer::WriteBuffer() {
    if (print_error_requests_) {
        std::cout.write(write_buffer_, write_buffer_pos_);
    } 
    if (has_output_path_) {
        output_stream_->write(write_buffer_, write_buffer_pos_);
    }
    write_buffer_pos_ = 0;
}
