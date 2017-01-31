//
//  DailyData.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 11/3/16.
//
//

#ifndef DailyData_hpp
#define DailyData_hpp

#include <istream>
#include <vector>
#include <map>


class DailyData {
public:
    typedef std::pair<std::string, int> DailyGameData;
    typedef std::pair<std::string, int> DailyDataKey;
    typedef std::vector<DailyGameData> DailyGameArray;
    
    std::map<DailyDataKey, DailyGameArray> dailyData;
    
public:
    size_t size() const;
    DailyGameArray getDailyGameArray(DailyDataKey key);
    
public:
    static DailyData defaultData();
    friend std::istream& operator>>(std::istream& IS, DailyData& DD);
    
    
};

#endif /* DailyData_hpp */
