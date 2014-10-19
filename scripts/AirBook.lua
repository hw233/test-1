--代码描述:此脚本仅处理天书奇缘的积分bug导致的情况 (被加载的地方似乎不合适，会被加载三次，影响效率)
print("XXX")
--os.execute("cat log/DB/TRACE20141007 |grep var|grep REPLACE |grep [^0-9]873[^0-9] |grep \"\\[00:[0,1,2,3,4,5][0-9]:\" > test1007.txt")  --需要修改cat语句已限制时间段
--os.execute("cat log/DB/TRACE20141007 |grep var|grep REPLACE |grep [^0-9]873[^0-9] |grep \"\\[01:[0,1,2,3][0-9]:\" >> test1007.txt")  --需要修改cat语句已限制时间段

--os.execute("cat log/DB/TRACE20141007bak |grep var|grep REPLACE |grep [^0-9]873[^0-9] |grep \"\\[00:[0,1,2,3,4,5][0-9]:\" > test1007.txt")  --需要修改cat语句已限制时间段
--os.execute("cat log/DB/TRACE20141007bak |grep var|grep REPLACE |grep [^0-9]873[^0-9] |grep \"\\[01:[0,1,2,3][0-9]:\" >> test1007.txt")  --需要修改cat语句已限制时间段

--os.execute("cat log/DBLog/TRACE20140911 |grep consume_gold |grep \",27,\"  > test1.txt")  --需要修改cat语句已限制时间段
--os.execute("cat log/DB/INFO20140707 |grep snow |grep REPLACE |grep \"\\[0[0-9]:\" > test1.txt")  --需要修改cat语句已限制时间段
--os.execute("cat log/DB/TRACE20140622 |grep var |grep [^0-9]599[^0-9] > test1.txt")
--os.execute("cat log/DB/TRACE20140622 |grep var |grep REPLACE |grep [^0-9]599[^0-9] |grep \"\\[1[0-9]\" > test1.txt")  --需要修改cat语句已限制时间段
--os.execute("awk '{print $11,$12,$13,$14}' test1.txt > test2.txt ")  --产生文件的格式为(XXXX, XXXX, XXXX)]
--reg = "values%((%d*),(%d*),(%d*),(%d*),(%d*),(%d*),(%d*)%)"
reg = "VALUES %((%d*), (%d*), (%d*), (%d*)%)"

file = io.open("test1007.txt","r")
--os.execute("cat log/DB/TRACE20140622 |grep var |grep [^0-9]599[^0-9] > test1.txt")
--os.execute("cat log/DB/TRACE20140622 |grep var |grep REPLACE |grep [^0-9]599[^0-9] |grep \"\\[1[0-9]\" > test1.txt")  --需要修改cat语句已限制时间段

--os.execute("awk '{print $11,$12,$13,$14}' test1.txt > test2.txt ")  --产生文件的格式为(XXXX, XXXX, XXXX)]
--file = io.open("test2.txt","r")
local res = {}
local result = {}
--获得此时间段 各玩家的积分变化情况
function getTheResult()
    for i in file:lines() do 
        first = string.find(i,",") 
        str1 = string.sub(i,2,first-1)   --获取玩家ID   （XXX,XXXX,XXXX,XXXX:玩家ID，数据ID,数值，结束时间）

        second = string.find(i,",",first+1) 
        str3 = string.sub(i,first+1,second - 1) --获取数据编号

        third = string.find(i,",",second+1) 
        str2 = string.sub(i,second+1,third - 1) --获取玩家数值
        if tonumber(str3) == 599 then    --数据编号指定

            if res[str1]== nil then    
                res[str1]= tonumber(str2)  --初始数值设为第一个出现的值
                result[str1]= 0    --结果增量设置为0
            end

            --条件刷分情况 10分的递进刷分，此时间内全部还原(处理刷分) ,问题(只要是10分递进的加分全部清楚)
            --print(str1..tonumber(str2))
            -- if  tonumber(str2) - res[str1] == 10 then 
            --     result[str1] = result[str1] - 10     --结果：削减的积分（10分，若无条件削减，参见下文增加）
            -- end

            --此判断开放：表示该时间段积分重新增加（处理积分意外清零的情况）
            if  tonumber(str2) > res[str1]  then 
                result[str1] = result[str1] + tonumber(str2) - res[str1]  --结果：增加积分（无条件增加）
            end

            res[str1] = tonumber(str2)
        else
            print(str3 .. "error")
        end
    end
    file:close()
end
function getTheResultInSnow2()
    for i in file:lines() do 
        x,y,a,b,c,d = string.find(i,reg)
        --print(x,y,a,b,c,d)
        if tonumber(b) == 873 then
            if result[a] == nil then
                result[a] = 0;
            else
                if tonumber(c) > res[a] then
                    result[a]= result[a] + tonumber(c) - res[a]   --结果增量设置为0
                    print(result[a])
                end
            end
            res[a] = tonumber(c)   --记录上一个值
        end
    end
    file:close()
end
function getTheResultInSnow()
    for i in file:lines() do 
        print(i)
        first = string.find(i,",") 
        str1 = string.sub(i,8,first-1)   --获取玩家ID   （XXX,XXXX,XXXX,XXXX:玩家ID，数据ID,数值，结束时间）

        second = string.find(i,",",first+1) 
        str2 = string.sub(i,first+1,second - 1) --获取数据编号

        third = string.find(i,",",second+1) 
        str3 = string.sub(i,second+1,third - 1) --获取玩家数值

        forth = string.find(i,")",third+1) 
        str4 = string.sub(i,third+1,forth - 1) --获取玩家数值

        if result[str4] == nil then
            result[str4] = 0
        end
        if tonumber(str1) > result[str4] then
            result[str4]= tonumber(str1)  --初始数值设为第一个出现的值
        end
        print(str1)
        print(str2)
        print(str3)
        print(str4)
    end
    file:close()
end

getTheResultInSnow2()

--写入sql语句完成数据还原
fileOpen = io.open("sql/updates/Object_20141007.txt","w")
--fileOpen = io.open("sql/updates/Object_0911.sql","w")

for key,value in pairs(result) do
    --local strs = string.format("UPDATE snow set score = score + %s where playerId = %s;",result[key],key)   --只影响总积分 
    --local strs = string.format("UPDATE var set data = data + %s where playerId = %s and id = 873;",result[key],key)   --只影响总积分 
    local strs = string.format("%s,%d",key,tonumber(result[key]))   --只影响总积分 
    print(strs)   --用于查看sql语句
    fileOpen:write(strs);
    fileOpen:write("\n")

    --pl = _GameActionLua:GetPlayerPtr(tonumber(key))
    --if pl == nil then
    --    return 
    --end
    --local value = pl:GetVar(846)
    --local value = pl:GetVar(84)
    --print("value:" .. value)
    --print("cha"..result[key])
    --if value < result[key] then 
    --   -- return 
    --end
    --if pl == nil then
    --    print("XXXX")
    --else  
    --    print("!!!!")
    --    pl:SetVar(846,value - result[key]);
    --    pl:SetVar(845,value - result[key]);
    --end
end
fileOpen:close()
