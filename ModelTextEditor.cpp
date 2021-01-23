//
// Created by kondr on 05.12.2020.
//

#include "ModelTextEditor.h"

#include <utility>

void ModelTextEditor::SetFilePath(string file_path)
{
    m_file_path_ = file_path;
    MyStringDecorator filename = m_file_path_;
    int slash_index = filename.ReversFind("/");
    if (slash_index == -1)
        m_file_name_ = m_file_path_;
    else
        m_file_name_ = string(filename.SubStr(slash_index + 1));
}

void ModelTextEditor::AddLine(string str)
{
    insert_line(m_lines_, str);
}

void ModelTextEditor::insert_line(DWORD index, string str)
{
    auto it = m_data_.begin();
    auto new_string = new MyStringDecorator(str);
    m_data_.insert(it + index, new_string);
    ++m_lines_;
}

void ModelTextEditor::erase_line(DWORD index)
{
    delete m_data_[index];
    auto it = m_data_.begin();
    m_data_.erase(it + index);
    --m_lines_;
    NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, nullptr);
}

DWORD ModelTextEditor::navigate_command(char command)
{
    if (command == -32)
    {
        COORD new_position;
        char additional_command = _getch();
        switch (additional_command) {
            case 75: //left
            {
                if (m_cursor_position_.X > 0)
                {
                    new_position.X = m_cursor_position_.X - 1;
                    new_position.Y = m_cursor_position_.Y;
                    move_cursor(new_position);
                }
                return 1;
            }
            case 77: //right
            {
                int flag = 0;
                if (m_insert_mode_)
                    ++flag;
                if (m_cursor_position_.X < static_cast<int>(m_data_[m_cursor_position_.Y]->length()) - 1 + flag)
                {
                    new_position.X = m_cursor_position_.X + 1;
                    new_position.Y = m_cursor_position_.Y;
                    move_cursor(new_position);
                }
                return 1;
            }
            case 72: //up
            {
                if (m_cursor_position_.Y > 0) {
                    if (m_data_[m_cursor_position_.Y - 1]->length() <= m_cursor_position_.X)
                    {
                        new_position.Y = m_cursor_position_.Y - 1;
                        if (m_data_[new_position.Y]->empty())
                            new_position.X = 0;
                        else
                            new_position.X = m_data_[new_position.Y]->length() - 1;
                    } else {
                        new_position.X = m_cursor_position_.X;
                        new_position.Y = m_cursor_position_.Y - 1;
                    }
                    move_cursor(new_position);
                }
                return 1;
            }
            case 80: //down
            {
                if (m_cursor_position_.Y < m_lines_ - 1) {
                    if (m_data_[m_cursor_position_.Y + 1]->length() <= m_cursor_position_.X) {
                        new_position.Y = m_cursor_position_.Y + 1;
                        if (m_data_[new_position.Y]->empty())
                            new_position.X = 0;
                        else
                            new_position.X = m_data_[new_position.Y]->length() - 1;
                    } else {
                        new_position.X = m_cursor_position_.X;
                        new_position.Y = m_cursor_position_.Y + 1;
                    }
                    move_cursor(new_position);
                }
                return 1;
            }
            case 73: //pageup
            {
                NotifyScreenBufferInfoUpdate(true);
                SHORT screen_height = cStatusLineNumber;
                if (m_cursor_position_.Y - screen_height >= 0)
                    move_cursor({0, static_cast<SHORT>(m_cursor_position_.Y - screen_height)});
                else
                    move_cursor(m_cursor_position_);
                return 1;
            }
            case 81: //pagedown
            {
                NotifyScreenBufferInfoUpdate(false);
                SHORT screen_height = cStatusLineNumber;
                if (m_cursor_position_.Y + screen_height < m_lines_)
                    move_cursor({0, static_cast<SHORT>(m_cursor_position_.Y + screen_height)});
                else
                    move_cursor(m_cursor_position_);
                return 1;
            }
            default:
                return 0;
        }
    }
    else
        return 0;
}

bool ModelTextEditor::navigate_help_file(char command)
{
    if (command == -32)
    {
        char additional_command = _getch();
        switch (additional_command)
        {
            case 72: //up
            {
                if (m_cursor_position_.Y > 0) move_cursor({0, static_cast<SHORT>(m_cursor_position_.Y - 1)});
                return true;
            }
            case 80: //down
            {
                if (m_cursor_position_.Y < 27)
                    move_cursor({0, static_cast<SHORT>(m_cursor_position_.Y + 1)});
                return true;
            }
            default:
                return false;
        }
    }
    else
        return false;
}

int ModelTextEditor::ExecuteCommand(const char *command)
{
    ofstream out;
    out.open("logfile.txt", ios_base::app);
    out << command << endl;
    out.close();
    switch (command[0])
    {
        case 'g': //gg
        {
            move_cursor({0, 0});
            return 1;
        }
        case 'd': //diw or dd
        {
            if (command[1] == 'i') //diw
            {
                if (m_data_[m_cursor_position_.Y]->empty())
                {
                    NotifyScreenBufferInfoUpdate(m_cursor_position_.Y + 1, m_lines_ - m_cursor_position_.Y - 1, -1);
                    erase_line(m_cursor_position_.Y);
                    move_cursor({0, m_cursor_position_.Y});
                }
                else {
                    WORD begin_index, end_index;
                    word_detect(begin_index, end_index);
                    if (((*m_data_[m_cursor_position_.Y])[m_cursor_position_.X] != ' ') && (end_index != m_data_[m_cursor_position_.Y]->length() - 1))
                        while ((end_index < m_data_[m_cursor_position_.Y]->length() - 1) && ((*m_data_[m_cursor_position_.Y])[end_index + 1] == ' '))
                            ++end_index;
                    m_data_[m_cursor_position_.Y]->Erase(begin_index, end_index - begin_index + 1);
                    NotifyLineUpdate(m_cursor_position_.Y, m_data_[m_cursor_position_.Y]->c_str());
                    move_cursor({static_cast<SHORT>(begin_index > 0 ? begin_index - 1 : 0), m_cursor_position_.Y});
                }
            }
            else{ //dd
                m_buffer_ = m_data_[m_cursor_position_.Y];
                NotifyScreenBufferInfoUpdate(m_cursor_position_.Y + 1, m_lines_ - m_cursor_position_.Y - 1, -1);
                erase_line(m_cursor_position_.Y);
                move_cursor({0, m_cursor_position_.Y});
            }
            m_edited_ = true;
            return 1;
        }
        case 'y': //yw
        {
            WORD begin_index, end_index;
            word_detect(begin_index, end_index);
            *m_buffer_ = m_data_[m_cursor_position_.Y]->SubStr(begin_index, end_index - begin_index + 1);
            return 1;
        }
        case ':':
        {
            WORD command_length = strlen(command);
            if (command_length == 1)
                return 1;
            WORD index = 0;
            char buffer[100] = "";
            switch(command[1])
            {
                case 'o':
                {
                    if (command_length > 2 && command[2] == ' ')
                    {
                        WORD i = 2;
                        while (command[++i] == ' '){}
                        if (command[i] != '\0')
                        {
                            if (m_edited_)
                            {
                                ofstream out;
                                out.open("logfile.txt", ios_base::app);
                                out << "File was edited but wasn't saved" << endl;
                                out.close();
                                NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, "File was edited but wasn't saved");
                                return 0;
                            }
                            while (command[i] != '\0')
                                buffer[index++] = command[i++];
                            ifstream in(buffer);
                            if (in.is_open())
                            {
                                m_cursor_position_ = {0, 0};
                                m_lines_ = 0;
                                NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, nullptr);
                                for (auto line : m_data_)
                                    delete line;
                                m_data_.clear();
                                m_file_path_ = buffer;
                                MyStringDecorator filename = m_file_path_;
                                int slash_index = filename.ReversFind("/");
                                if (slash_index == -1)
                                    m_file_name_ = m_file_path_;
                                else
                                    m_file_name_ = string(filename.SubStr(slash_index + 1));
                                NotifyNewFileUpdate(in, m_file_path_);
                                in.close();
                            }
                            else{
                                ofstream out;
                                out.open("logfile.txt", ios_base::app);
                                out << "File not found" << endl;
                                out.close();
                                NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, "File not found");
                                return 0;
                            }
                        }
                    }
                    break;
                }
                case 'x':
                {
                    if (command_length == 2)
                    {
                        save_file();
                        return -1;
                    }
                    else
                        return 0;
                }
                case 'w':
                {
                    if (command_length == 2) //:w
                    {
                        save_file();
                        break;
                    }
                    else if ((command_length > 3) && (command[2] == ' ')) //:w filename
                    {
                        WORD i = 2;
                        while (command[++i] == ' '){}
                        if (command[i] == '\0')
                            return 0;
                        while (command[i] != '\0')
                            buffer[index++] = command[i++];
                        ofstream try_out(buffer);
                        if (try_out.is_open())
                        {
                            try_out.close();
                            m_file_path_ = buffer;
                            MyStringDecorator filename = m_file_path_;
                            int slash_index = filename.ReversFind("/");
                            if (slash_index == -1)
                                m_file_name_ = m_file_path_;
                            else
                                m_file_name_ = string(filename.SubStr(slash_index + 1));
                            save_file();
                            NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, nullptr);
                            break;
                        }
                        else
                        {
                            NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, "Open file error");
                            ofstream out;
                            out.open("logfile.txt", ios_base::app);
                            out << "Open file error" << endl;
                            out.close();
                            return 0;
                        }
                    }
                    else if ((command_length == 4) && (command[2] == 'q') && (command[3] == '!')) //:wq!
                    {
                        save_file();
                        return -1;
                    }
                    else
                        return 0;
                }
                case 'q':
                {
                    if (command_length == 2) //:q
                    {
                        if (m_edited_)
                        {
                            ofstream out;
                            out.open("logfile.txt", ios_base::app);
                            out << "File was edited but wasn't saved" << endl;
                            out.close();
                            NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, "File was edited but wasn't saved");
                            return 0;
                        }
                        return -1;
                    }
                    else if ((command_length == 3) && (command[2] == '!')) //:q!
                        return -1;
                    else
                        return 0;
                }
                case 'h':
                {
                    if (command_length == 2)
                    {
                        COORD saved_position = m_cursor_position_;
                        m_cursor_position_ = {0, 0};
                        help_mode();
                        for (auto line : m_data_)
                        {
                            cout << *line << endl;
                        }
                        move_cursor(saved_position);
                    }
                    else return 0;
                    break;
                }
                default:
                {
                    WORD i = 1;
                    while(command[i] != '\0')
                    {
                        if (!isdigit(command[i]))
                            return 0;
                        else
                            buffer[index++] = command[i];
                        ++i;
                    }
                    WORD line_number = atoi(buffer);
                    line_number = (line_number > m_lines_) ? m_lines_ : line_number;
                    move_cursor({0, static_cast<SHORT>(line_number - 1)});
                }
            }
            return 1;
        }
        case '?':
        case '/':
        {
            if (command[1] == '\0')
                return 0;
            WORD index = 0;
            char buffer[100] = "";
            while(command[index + 1] != '\0')
            {
                buffer[index] = command[index + 1];
                ++index;
            }
            if (command[0] == '/')
                direct_search(string(buffer));
            else
                revers_search(string(buffer));
            return 1;
        }
        default: //NG
        {
            char temp[20] = "";
            strcpy_s(temp, command);
            temp[strlen(command) - 1] = '\0';
            DWORD number = atoi(temp);
            number = (number > m_lines_) ? m_lines_ : number;
            move_cursor({0, static_cast<SHORT>(number - 1)});
            return 1;
        }
    }
}

int ModelTextEditor::ExecuteCommand(char command)
{
    COORD new_position;
    if (navigate_command(command))
        return 1;
    ofstream out;
    out.open("logfile.txt", ios_base::app);
    out << command << endl;
    out.close();
    switch (command)
    {
        case '^': //move cursor to the begin of line
        case '0':
        {
            new_position.X = 0;
            new_position.Y = m_cursor_position_.Y;
            move_cursor(new_position);
            return 1;
        }
        case '$': //move cursor to the end of line
        {
            if (m_data_[m_cursor_position_.Y]->length() > 0)
            {
                new_position.X = m_data_[m_cursor_position_.Y]->length() - 1;
                new_position.Y = m_cursor_position_.Y;
                move_cursor(new_position);
            }
            return 1;
        }
        case 'w': //move cursor to the end of next word
        {
           WORD begin_index, end_index;
           COORD old_position = m_cursor_position_;
           bool start_position = true;
           while (true) {
               if (m_data_[m_cursor_position_.Y]->length() == 0)  //current line is empty
               {
                   if (m_cursor_position_.Y == m_lines_ - 1) //current line is the last line in file
                   {
                       move_cursor(old_position);
                       break;
                   }
                   move_cursor({0, static_cast<SHORT>(m_cursor_position_.Y + 1)});
                   start_position = false;
                   continue;
               }
               word_detect(begin_index, end_index);
               if ((*m_data_[m_cursor_position_.Y])[m_cursor_position_.X] == ' ') //current cursor position between words
               {
                   if (end_index == m_data_[m_cursor_position_.Y]->length() - 1) //there's no any next words in current line
                   {
                       if (m_cursor_position_.Y == m_lines_ - 1) //current line is the last line in file
                       {
                           move_cursor(old_position);
                           break;
                       }
                       move_cursor({0, static_cast<SHORT>(m_cursor_position_.Y + 1)});
                       start_position = false;
                       continue;
                   }
                   else //there is at least one next word in current line
                   {
                       move_cursor({static_cast<SHORT>(end_index + 1), m_cursor_position_.Y});
                       word_detect(begin_index, end_index);
                       move_cursor({static_cast<SHORT>(end_index), m_cursor_position_.Y});
                       break;
                   }
               }
               else //current cursor position on word
               {
                   if (end_index == m_cursor_position_.X) //current position in the end of word
                   {
                        if (!start_position)
                            break;
                        if (m_cursor_position_.X == m_data_[m_cursor_position_.Y]->length() - 1) //current position in the end of line
                        {
                            if (m_cursor_position_.Y == m_lines_ - 1) //current line is the last line in file
                            {
                                move_cursor(old_position);
                                break;
                            }
                            move_cursor({0, static_cast<SHORT>(m_cursor_position_.Y + 1)});
                            start_position = false;
                            continue;
                        }
                        else
                        {
                            move_cursor({static_cast<SHORT>(m_cursor_position_.X + 1), m_cursor_position_.Y});
                            start_position = false;
                            continue;
                        }
                   }
                   else //current position in the middle (or beginning) of word
                   {
                       move_cursor({static_cast<SHORT>(end_index), m_cursor_position_.Y});
                       break;
                   }
               }
           }
           return 1;
        }
        case 'b': //move cursor to the begin of previous word
        {
            WORD begin_index, end_index;
            COORD old_position = m_cursor_position_;
            bool start_position = true;
            while (true) {
                if (m_data_[m_cursor_position_.Y]->length() == 0)  //current line is empty
                {
                    if (m_cursor_position_.Y == 0) //current line is the first line in file
                    {
                        move_cursor(old_position);
                        break;
                    }
                    move_cursor({static_cast<SHORT>(m_data_[m_cursor_position_.Y - 1]->length() - 1), static_cast<SHORT>(m_cursor_position_.Y - 1)});
                    start_position = false;
                    continue;
                }
                word_detect(begin_index, end_index);
                if ((*m_data_[m_cursor_position_.Y])[m_cursor_position_.X] == ' ') //current cursor position between words
                {
                    if (begin_index == 0) //there's no any previous words in current line
                    {
                        if (m_cursor_position_.Y == 0) //current line is the first line in file
                        {
                            move_cursor(old_position);
                            break;
                        }
                        move_cursor({static_cast<SHORT>(m_data_[m_cursor_position_.Y - 1]->length() - 1), static_cast<SHORT>(m_cursor_position_.Y - 1)});
                        start_position = false;
                        continue;
                    }
                    else //there is at least one previous word in current line
                    {
                        move_cursor({static_cast<SHORT>(begin_index - 1), m_cursor_position_.Y});
                        word_detect(begin_index, end_index);
                        move_cursor({static_cast<SHORT>(begin_index), m_cursor_position_.Y});
                        break;
                    }
                }
                else //current cursor position on word
                {
                    if (begin_index == m_cursor_position_.X) //current position in the beginning of word
                    {
                        if (!start_position)
                            break;
                        if (m_cursor_position_.X == 0) //current position in the beginning of line
                        {
                            if (m_cursor_position_.Y == 0) //current line is the first line in file
                            {
                                move_cursor(old_position);
                                break;
                            }
                            move_cursor({static_cast<SHORT>(m_data_[m_cursor_position_.Y - 1]->length() - 1), static_cast<SHORT>(m_cursor_position_.Y - 1)});
                            start_position = false;
                            continue;
                        }
                        else
                        {
                            move_cursor({static_cast<SHORT>(m_cursor_position_.X - 1), m_cursor_position_.Y});
                            start_position = false;
                            continue;
                        }
                    }
                    else //current position in the middle (or beginning) of word
                    {
                        move_cursor({static_cast<SHORT>(begin_index), m_cursor_position_.Y});
                        break;
                    }
                }
            }
            return 1;
        }
        case 'G':
        {
            new_position.Y = m_lines_ - 1;
            new_position.X = m_data_[new_position.Y]->length() - 1;
            move_cursor(new_position);
            return 1;
        }
        case 'x':
        {
            m_data_[m_cursor_position_.Y]->Erase(m_cursor_position_.X, 1);
            new_position = m_cursor_position_;
            NotifyLineUpdate(m_cursor_position_.Y, m_data_[m_cursor_position_.Y]->c_str());
            move_cursor(new_position);
            m_edited_ = true;
            return 1;
        }
        case 'y':
        {
            *m_buffer_ = m_data_[m_cursor_position_.Y]->c_str();
            return 1;
        }
        case 'p':
        {
            if (m_data_[m_cursor_position_.Y]->empty())
            {
                m_data_[m_cursor_position_.Y]->Insert(0, m_buffer_->c_str());
                new_position = {static_cast<SHORT>(m_cursor_position_.X + m_buffer_->length() - 1), m_cursor_position_.Y};
            }
            else {
                m_data_[m_cursor_position_.Y]->Insert(m_cursor_position_.X + 1, m_buffer_->c_str());
                new_position = {static_cast<SHORT>(m_cursor_position_.X + m_buffer_->length()), m_cursor_position_.Y};
            }
            NotifyLineUpdate(m_cursor_position_.Y, m_data_[m_cursor_position_.Y]->c_str());
            move_cursor(new_position);
            if (!m_buffer_->empty())
                m_edited_ = true;
            return 1;
        }
        case 'i':
        {
            m_insert_mode_ = true;
            insert_mode();
            return 1;
        }
        case 'I':
        {
            move_cursor({0, m_cursor_position_.Y});
            m_insert_mode_ = true;
            insert_mode();
            return 1;
        }
        case 'A':
        {
            move_cursor({static_cast<SHORT>(m_data_[m_cursor_position_.Y]->length()), m_cursor_position_.Y});
            m_insert_mode_ = true;
            insert_mode();
            return 1;
        }
        case 'S':
        {
            m_data_[m_cursor_position_.Y]->clear();
            move_cursor({0, m_cursor_position_.Y});
            cout << "\33[2K\r";
            m_insert_mode_ = true;
            insert_mode();
            return 1;
        }
        case 'r':
        {
            char symbol = _getch();
            if ((symbol == 27) || (symbol == -32) || (symbol == 13))
                return 1;
            string arg(1, symbol);
            m_data_[m_cursor_position_.Y]->Replace(m_cursor_position_.X, 1, arg);
            SHORT x = m_cursor_position_.X;
            NotifyLineUpdate(m_cursor_position_.Y, m_data_[m_cursor_position_.Y]->c_str());
            move_cursor({x, m_cursor_position_.Y});
            m_edited_ = true;
            return 1;
        }
        case 'n':
        {
            if (!m_revers_search_)
                direct_search(m_last_search_);
            else
                revers_search(m_last_search_);
            return 1;
        }
        case 'N':
        {
            if (m_revers_search_)
                direct_search(m_last_search_);
            else
                revers_search(m_last_search_);
            return 1;
        }
        default:
            return 0;
    }
}

bool ModelTextEditor::insert_handler(char symbol)
{
    if (symbol == 27) //esc
    {
        m_insert_mode_ = false;
        return false;
    }
    if (navigate_command(symbol))
        return true;
    COORD new_position = m_cursor_position_;
    if (symbol == 8) //backspace
    {
        if (m_cursor_position_.X == 0)
        {
            if (m_cursor_position_.Y > 0)
            {
                new_position = {static_cast<SHORT>(m_data_[m_cursor_position_.Y - 1]->length()), static_cast<SHORT>(m_cursor_position_.Y - 1)};
                m_data_[m_cursor_position_.Y - 1]->Append(m_data_[m_cursor_position_.Y]->c_str());
                NotifyScreenBufferInfoUpdate(m_cursor_position_.Y + 1, m_lines_ - m_cursor_position_.Y - 1, -1);
                erase_line(m_cursor_position_.Y);
                NotifyLineUpdate(m_cursor_position_.Y - 1, m_data_[m_cursor_position_.Y - 1]->c_str());
                m_edited_ = true;
            }
        }
        else {
            m_data_[m_cursor_position_.Y]->Erase(m_cursor_position_.X - 1, 1);
            new_position = {static_cast<SHORT>(m_cursor_position_.X - 1), m_cursor_position_.Y};
            NotifyLineUpdate(m_cursor_position_.Y, m_data_[m_cursor_position_.Y]->c_str());
            m_edited_ = true;
        }
    }
    else if (symbol == 13) //enter
    {
        NotifyScreenBufferInfoUpdate(m_cursor_position_.Y + 1, m_lines_ - m_cursor_position_.Y - 1, 1);
        insert_line(m_cursor_position_.Y + 1, string(m_data_[m_cursor_position_.Y]->SubStr(m_cursor_position_.X)));
        m_data_[m_cursor_position_.Y]->Erase(m_cursor_position_.X, m_data_[m_cursor_position_.Y + 1]->length());
        new_position = {0, static_cast<SHORT>(m_cursor_position_.Y + 1)};
        NotifyLineUpdate(m_cursor_position_.Y, m_data_[m_cursor_position_.Y]->c_str());
        NotifyLineUpdate(m_cursor_position_.Y + 1, m_data_[m_cursor_position_.Y + 1]->c_str());
        NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, nullptr);
        m_edited_ = true;
    }
    else {
        m_data_[m_cursor_position_.Y]->Insert(m_cursor_position_.X, 1, symbol);
        new_position = {static_cast<SHORT>(m_cursor_position_.X + 1), m_cursor_position_.Y};
        NotifyLineUpdate(m_cursor_position_.Y, m_data_[m_cursor_position_.Y]->c_str());
        m_edited_ = true;
    }
    move_cursor(new_position);
    return true;
}

bool ModelTextEditor::help_handler(char symbol)
{
    if (symbol == 27) //esc
        return false;
    if (navigate_help_file(symbol))
        return true;
    if (symbol == ':') {
        char q_symbol = _getch();
        if (q_symbol == 'q') {
            char return_symbol = _getch();
            if (return_symbol == 13)
                return false;
        }
    }
    return true;

}

const char * ModelTextEditor::m_getLine(SHORT index)
{
    if (index >= m_lines_)
        return "";
    return m_data_[index]->c_str();
}

void ModelTextEditor::AddSubscriber(ISubscriber* subscriber)
{
    m_pSubscriber_ = subscriber;
}

void ModelTextEditor::RemoveSubscriber()
{
    m_pSubscriber_ = nullptr;
}

void ModelTextEditor::NotifyCursorUpdate(COORD position)
{
    m_pSubscriber_->CursorUpdate(position);
}

void ModelTextEditor::NotifyStatusBarUpdate(SHORT lines_total, string filename, bool insert_mode, const char* message)
{
    m_pSubscriber_->StatusBarUpdate(lines_total, filename, insert_mode, message);
}

void ModelTextEditor::NotifyScreenBufferInfoUpdate(bool up)
{
    m_pSubscriber_->ScreenBufferInfoUpdate(up);
}

void ModelTextEditor::NotifyScreenBufferInfoUpdate(SHORT start_line, SHORT lines_count, SHORT shift)
{
    m_pSubscriber_->ScreenBufferInfoUpdate(start_line, lines_count, shift);
}

void ModelTextEditor::NotifyLineUpdate(SHORT index, const char* line)
{
    m_pSubscriber_->LineUpdate(index, line);
}

void ModelTextEditor::NotifyNewFileUpdate(ifstream& in, string& filepath)
{
    m_pSubscriber_->NewFileUpdate(in, filepath);
}

void ModelTextEditor::move_cursor(COORD position)
{
    NotifyCursorUpdate(position);
    m_cursor_position_ = position;
}

void ModelTextEditor::word_detect(WORD &begin_index, WORD &end_index)
{
    if (m_data_[m_cursor_position_.Y]->length() == 0)
    {
        begin_index = 0;
        end_index = 0;
        return;
    }
    begin_index = m_cursor_position_.X;
    end_index = m_cursor_position_.X;
    char symbol = (*m_data_[m_cursor_position_.Y])[m_cursor_position_.X];
    if (symbol == ' ')
    {
        while(begin_index > 0)
        {
            if((*m_data_[m_cursor_position_.Y])[begin_index - 1] != ' ')
                break;
            --begin_index;
        }
        while(end_index < m_data_[m_cursor_position_.Y]->length() - 1)
        {
            if((*m_data_[m_cursor_position_.Y])[end_index + 1] != ' ')
                break;
            ++end_index;
        }
    }
    else{
        while(begin_index > 0)
        {
            if((*m_data_[m_cursor_position_.Y])[begin_index - 1] == ' ')
                break;
            --begin_index;
        }
        while(end_index < m_data_[m_cursor_position_.Y]->length() - 1)
        {
            if((*m_data_[m_cursor_position_.Y])[end_index + 1] == ' ')
                break;
            ++end_index;
        }
    }
}

DWORD ModelTextEditor::save_file()
{
    ofstream out(m_file_path_);
    if (out.is_open())
    {
        for (auto line : m_data_)
        {
            out << *line << endl;
        }
        out.close();
        m_edited_ = false;
        return 1;
    }
    else
        return 0;
}

void ModelTextEditor::direct_search(const string& search_string)
{
    m_revers_search_ = false;
    m_last_search_ = search_string;
    int index = m_data_[m_cursor_position_.Y]->Find(search_string, m_cursor_position_.X + 1);
    if (index >= 0)
        move_cursor({static_cast<SHORT>(index), m_cursor_position_.Y});
    else
    {
        DWORD line_number = m_cursor_position_.Y + 1;
        while (line_number < m_lines_)
        {
            index = m_data_[line_number]->Find(search_string);
            if (index >= 0)
            {
                move_cursor({static_cast<SHORT>(index), static_cast<SHORT>(line_number)});
                break;
            }
            ++line_number;
        }
    }
}

void ModelTextEditor::revers_search(const string& search_string)
{
    m_revers_search_ = true;
    m_last_search_ = search_string;
    int index = m_data_[m_cursor_position_.Y]->ReversFind(search_string, m_cursor_position_.X - 1);
    if (index >= 0)
        move_cursor({static_cast<SHORT>(index), m_cursor_position_.Y});
    else if (m_cursor_position_.Y > 0)
    {
        int line_number = m_cursor_position_.Y - 1;
        while (line_number >= 0)
        {
            index = m_data_[line_number]->ReversFind(search_string);
            if (index >= 0)
            {
                move_cursor({static_cast<SHORT>(index), static_cast<SHORT>(line_number)});
                break;
            }
            --line_number;
        }
    }
}

void ModelTextEditor::insert_mode()
{
    NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, nullptr);
    char symbol;
    while (true)
    {
        symbol = _getch();
        if (!insert_handler(symbol))
            break;
    }
    NotifyStatusBarUpdate(m_lines_, m_file_name_, m_insert_mode_, nullptr);
}

WORD ModelTextEditor::help_mode()
{
    ifstream in("help.txt");
    if (in.is_open())
    {
        string old_filename = m_file_name_;
        DWORD old_lines_total = m_lines_;
        system("cls");
        string line;
        while (getline(in, line))
        {
            cout << line << endl;
        }
        in.close();
        NotifyStatusBarUpdate(28, string("help.txt"), false, nullptr);
        move_cursor({ 0, 0 });
        char symbol;
        while (true)
        {
            symbol = _getch();
            if (!help_handler(symbol))
            {
                NotifyStatusBarUpdate(old_lines_total, old_filename, false, nullptr);
                system("cls");
                return 1;
            }
        }
    }
    else return 0;
}