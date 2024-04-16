#ifndef __YUGIOHFIELDDATA_H__
#define __YUGIOHFIELDDATA_H__

#include <iostream>
#include <fstream>

#include "../../utility/json.hpp"

class YuGiOhFieldData
{
public:
	YuGiOhFieldData();
    ~YuGiOhFieldData();
	void InitialDefault();
	YuGiOhFieldData&operator=(YuGiOhFieldData & rhs);
	bool operator==(YuGiOhFieldData const& rhs);
	bool operator!=(YuGiOhFieldData const& rhs);
	friend std::ostream& operator<<(std::ostream& stdOutput, YuGiOhFieldData& rhs);
	friend std::ostream& operator<<(std::ofstream& outputFile, YuGiOhFieldData& rhs);
	void StoreValueFromFieldPositionStructNameForJson(int index, int value);

private:
	nlohmann::json board;

protected:
};

#endif // __YUGIOHFIELDDATA_H__

