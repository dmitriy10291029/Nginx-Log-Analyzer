#include <iostream>
#include <cstring>
#include <algorithm>

#include "log_analyzer.hpp"
#include "../utils/print_util.hpp" 
#include "../utils/time_util.hpp"


void LogAnalyzer::SetUp(int argc, char** argv) {
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
    temp_file_ = new std::fstream("__temp__.txt", std::ios::in | std::ios::out | std::ios::trunc);
    if (!temp_file_->is_open()) {
        has_output_path_ = false;
        PrintUtil::PrintlnWarn("Can't create and open temp file for stats option.");
        PrintUtil::PrintlnWarn("Stats option was disabled.");
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
    has_lower_time_bound_ = opt.declared && opt.value_declared;
    if (opt.value_declared) {
        lower_time_bound_unix_ = std::atol(opt.value);
    }
}

void LogAnalyzer::SetUpTo() {
    auto opt = args_parser_.GetOption("to").value();
    has_upper_time_bound_ = opt.declared && opt.value_declared;
    if (opt.value_declared) {
        upper_time_bound_unix_ = std::atol(opt.value);
        if (upper_time_bound_unix_ < lower_time_bound_unix_) {
            PrintUtil::PrintlnWarn("Invalid time borders. This options was disabled.");
            has_lower_time_bound_ = false;
            has_upper_time_bound_ = false;
        }
    }
}


int LogAnalyzer::Run() {
    if (err_code_ != 0) {
        return err_code_;
    }

    if (lower_time_bound_unix_) {
        requests_total_++;
        FindLowerTimeBound();
        WriteRequestIfError();
    }

    while (!ReachedEnd()) {
        requests_total_++;
        ReadRequest();
        if (found_upper_time_bound_) {
            break;
        }
        WriteRequestIfError();
    }
    WriteBuffer();

    if (invalid_amount_ > 0) {
        PrintUtil::PrintlnWarn("Requests containing errors were detected:");
        PrintUtil::PrintlnWarn(invalid_amount_);
        PrintUtil::Println();
    }

    PrintMostFreqErrReq();

    PrintUtil::PrintlnInfo("Total processed requests:");
    PrintUtil::PrintlnInfo(requests_total_);
    PrintUtil::Println();

    PrintUtil::PrintlnInfo("Server error requests:");
    PrintUtil::PrintlnInfo(err_amount_);
    PrintUtil::Println();

    return 0;
}

int LogAnalyzer::GetTotalRequests() const {
    return requests_total_;
}

int LogAnalyzer::GetInvalidRequests() const {
    return invalid_amount_;
}

int LogAnalyzer::GetErrorRequests() const {
    return err_amount_;
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
    curr_request_.datetime_unix = line_pos_;
    FindInLine(']');
    IncrementLinePos();
    SplitLine();
    AssertUpperTimeBound();

    // parse curr_request url
    FindInLine('\"');
    curr_request_.url = line_pos_;
    IncrementLinePos();
    FindInLineFromEnd('\"');
    IncrementLinePos();
    SplitLine();

    // parse status code
    curr_request_.status_str = line_pos_;
    FindInLine(' ');
    SplitLine();
    if (!IsCorrectStatus(curr_request_.status_str)) {
        err_code_ = 1;
    }

    // parse read bytes
    curr_request_.bytes_str = line_pos_;
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
            if (line_size_ * 2 > lineLenMax) {
                err_code_ = 1;
                delete line_;
                line_ = nullptr;

                return 0;
            }
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

void LogAnalyzer::FindLowerTimeBound() {
    if (!has_lower_time_bound_) {
        err_code_ = 1;
        return;
    }

    while (!ReachedEnd()) {
        ReadRequest();
        if (err_code_ != 0) {
            err_code_ = 0;
            continue;
        }
        auto datetime = TimeUtil::ConvertLogsTimeToUnix(curr_request_.datetime_unix);
        if (datetime != TimeUtil::convertingErrorCode) {
            if (datetime >= lower_time_bound_unix_) {
                return;
            } 
        }
    }
}

void LogAnalyzer::AssertUpperTimeBound() {
    if (err_code_ == 0 && has_upper_time_bound_) {
        auto datetime = TimeUtil::ConvertLogsTimeToUnix(curr_request_.datetime_unix);
        if (datetime == TimeUtil::convertingErrorCode) {
            err_code_ = 1;

        } else if (datetime > upper_time_bound_unix_) {
            found_upper_time_bound_ = true;
        }
    }
}

bool LogAnalyzer::IsWhitespace(char ch) {
    return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

bool LogAnalyzer::IsCorrectStatus(const char* status) {
    return err_code_ == 0 && status != nullptr && strlen(status) == 3 
        && status[0] >= '2' && status[0] <= '5' 
        && std::isdigit(status[1]) && std::isdigit(status[2]);
}

bool LogAnalyzer::IsServerErrorStatus(const char* status) {
    return status[0] == '5';
}

void LogAnalyzer::Write(const char* text) {
    if (text == nullptr) {
        return;
    }
    while (*text != '\0') {
        write_buffer_[write_buffer_pos_] = *text;
        ++text;
        ++write_buffer_pos_;
        if (write_buffer_pos_ == writeBufferSize) {
            WriteBuffer();
        }
    }
}

void LogAnalyzer::WriteRequest() {
    Write(std::to_string(err_amount_).c_str());
    Write(": ");
    Write(curr_request_.remote_addr);
    Write(" - - ");
    Write(curr_request_.datetime_unix);
    Write(" ");
    Write(curr_request_.url);
    Write(" ");
    Write(curr_request_.status_str);
    Write(" ");
    Write(curr_request_.bytes_str);
    Write("\n");
}

void LogAnalyzer::WriteRequestIfError() {
    if (err_code_ != 0) {
        err_code_ = 0;
        ++invalid_amount_;
        
    } else if (IsServerErrorStatus(curr_request_.status_str)) {
        ++err_amount_;

        if (print_most_freq_err_requests_) {
            auto hash = CalcStrHash(curr_request_.url);
            
            bool is_entry_first = true;

            if (req_freq_map.contains(hash)) {
                auto range = req_freq_map.equal_range(hash);
                for (auto it = range.first; it != range.second; ++it) {
                    if (TempFileFindAndCompare(it->second.second, curr_request_.url)) {
                        it->second.first++;
                        is_entry_first = false;
                        break;
                    }
                }
            }

            if (is_entry_first) {
                auto pos = TempFileAddRequest(curr_request_.url);
                req_freq_map.insert({hash, {1, pos}});
            }
        }

        WriteRequest();
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

uint64_t LogAnalyzer::CalcStrHash(const char* str) {
    uint64_t hashValue = 0;
    uint64_t pPow = 1;

    size_t len = std::strlen(str);
    for (size_t it = 0; it < len; ++it) {
        hashValue = (hashValue + (str[it] - 'a' + 1) * pPow) % hashFuncModule; 
        pPow = (pPow * hashFuncBase) % hashFuncModule;
    }

    return hashValue;
}

std::streampos LogAnalyzer::TempFileAddRequest(const char* str) {
    temp_file_->seekp(0, std::ios::end); // erase it?
    std::streampos position = temp_file_->tellp();
    *temp_file_ << str << std::endl;

    return position;
}

const char* LogAnalyzer::TempFileFind(std::streampos pos) {
    temp_file_->seekg(pos, std::ios::beg);
    char* result = new char[lineLenMax];
    temp_file_->getline(result, lineLenMax);

    return result;
}

bool LogAnalyzer::TempFileFindAndCompare(std::streampos pos, const char* str) {
    temp_file_->seekg(pos, std::ios::beg);
    
    char ch_file; 
    size_t i = 0; 

    while (true) {
        temp_file_->get(ch_file);  

        if (ch_file == '\n' || ch_file == EOF || ch_file == '\0') {
            return str[i] == '\0';  

        } else if (str[i] == '\0') {
            return false;

        } else if (ch_file != str[i]) {
            return false;
        }

        i++;  
    }
}

void LogAnalyzer::PrintMostFreqErrReq() {
    if (!print_error_requests_) {
        return;
    }

    std::vector<std::pair<int, int>> pairs;

    for (const auto& pair : req_freq_map) {
        pairs.push_back(pair.second);
    }

    std::sort(pairs.begin(), pairs.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.first > b.first;
    });

    std::size_t count = 0;
    for (const auto& pair : pairs) {
        if (count >= most_freq_err_requests_len_) {
            break;
        }
        
        const char* url = TempFileFind(pair.second);
        int amount = pair.first;
        PrintUtil::PrintlnInfo(url);
        PrintUtil::PrintlnInfo(amount);
        PrintUtil::Println();
        delete url;

        count++;
    }
}
