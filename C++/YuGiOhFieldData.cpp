#include "YuGiOhFieldData.h"
#include "FieldPositionData.h"

using Json = nlohmann::json;

YuGiOhFieldData::YuGiOhFieldData()
{

};

YuGiOhFieldData::~YuGiOhFieldData()
{
    this->board.clear();
};

void YuGiOhFieldData::InitialDefault()
{
    for(int i=0;i<sizeof(struct FieldPosition)/sizeof(struct ACardPosition);i++)
        StoreValueFromFieldPositionStructNameForJson(i, 0);
};

// Deep Copy
YuGiOhFieldData& YuGiOhFieldData::operator=(YuGiOhFieldData & rhs)
{
    this->board = rhs.board;
    return *this;
}

// Already impliment in jsoncpp
bool YuGiOhFieldData::operator==(YuGiOhFieldData const& rhs)
{
    return this->board == rhs.board;
}

// Already impliment in jsoncpp
bool YuGiOhFieldData::operator!=(YuGiOhFieldData const& rhs)
{
    return this->board != rhs.board;
}

// "<<" operator: copy jsoncpp to outputFile
std::ostream& operator<<(std::ostream& stdOutput, YuGiOhFieldData& rhs)
{
        stdOutput << std::setw(4) << rhs.board << std::endl;
        return stdOutput;
};

// "<<" operator: print jsoncpp to standard output
std::ostream& operator<<(std::ofstream& outputFile, YuGiOhFieldData& rhs)
{
    outputFile << std::setw(4) << rhs.board << std::endl;
    return outputFile;
};

void YuGiOhFieldData::StoreValueFromFieldPositionStructNameForJson(int index, int value)
{
    switch(index)
    {
        case 0:
            board[FieldPositionStructNameForJson[0]][0] = value;
        break;
        case 1:
            board[FieldPositionStructNameForJson[0]][1] = value;
        break;
        case 2:
            board[FieldPositionStructNameForJson[1]][0] = value;
        break;
        case 3:
            board[FieldPositionStructNameForJson[1]][1] = value;
        break;
        case 4:
            board[FieldPositionStructNameForJson[2]][0] = value;
        break;
        case 5:
            board[FieldPositionStructNameForJson[2]][1] = value;
        break;
        case 6:
            board[FieldPositionStructNameForJson[3]][0] = value;
        break;
        case 7:
            board[FieldPositionStructNameForJson[3]][1] = value;
        break;
        case 8:
            board[FieldPositionStructNameForJson[4]][0] = value;
        break;
        case 9:
            board[FieldPositionStructNameForJson[4]][1] = value;
        break;
        case 10:
            board[FieldPositionStructNameForJson[4]][2] = value;
        break;
        case 11:
            board[FieldPositionStructNameForJson[4]][3] = value;
        break;
        case 12:
            board[FieldPositionStructNameForJson[4]][4] = value;
        break;
        case 13:
            board[FieldPositionStructNameForJson[5]][0] = value;
        break;
        case 14:
            board[FieldPositionStructNameForJson[5]][1] = value;
        break;
        case 15:
            board[FieldPositionStructNameForJson[5]][2] = value;
        break;
        case 16:
            board[FieldPositionStructNameForJson[5]][3] = value;
        break;
        case 17:
            board[FieldPositionStructNameForJson[5]][4] = value;
        break;
        case 18:
            board[FieldPositionStructNameForJson[6]][0] = value;
        break;
        case 19:
            board[FieldPositionStructNameForJson[6]][1] = value;
        break;
        case 20:
            board[FieldPositionStructNameForJson[6]][2] = value;
        break;
        case 21:
            board[FieldPositionStructNameForJson[6]][3] = value;
        break;
        case 22:
            board[FieldPositionStructNameForJson[6]][4] = value;
        break;
        case 23:
            board[FieldPositionStructNameForJson[7]][0] = value;
        break;
        case 24:
            board[FieldPositionStructNameForJson[7]][1] = value;
        break;
        case 25:
            board[FieldPositionStructNameForJson[7]][2] = value;
        break;
        case 26:
            board[FieldPositionStructNameForJson[7]][3] = value;
        break;
        case 27:
            board[FieldPositionStructNameForJson[7]][4] = value;
        break;
        case 28:
            board[FieldPositionStructNameForJson[8]][0] = value;
        break;
        case 29:
            board[FieldPositionStructNameForJson[8]][1] = value;
        break;
        case 30:
            board[FieldPositionStructNameForJson[8]][2] = value;
        break;
        case 31:
            board[FieldPositionStructNameForJson[8]][3] = value;
        break;
        case 32:
            board[FieldPositionStructNameForJson[8]][4] = value;
        break;
        case 33:
            board[FieldPositionStructNameForJson[9]][0] = value;
        break;
        case 34:
            board[FieldPositionStructNameForJson[9]][1] = value;
        break;
        case 35:
            board[FieldPositionStructNameForJson[9]][2] = value;
        break;
        case 36:
            board[FieldPositionStructNameForJson[9]][3] = value;
        break;
        case 37:
            board[FieldPositionStructNameForJson[9]][4] = value;
        break;
    }
}