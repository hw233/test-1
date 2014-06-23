--代码描述:此脚本仅处理天书奇缘的积分bug导致的情况 (被加载的地方似乎不合适，会被加载三次，影响效率)
print("XXX")
os.execute("zcat log/DB/TRACE20140622.gz |grep var |grep [^0-9]599[^0-9] |grep \"\\[0[0,1]:\" > test1.txt")  --需要修改cat语句已限制时间段
--os.execute("cat log/DB/TRACE20140622 |grep var |grep [^0-9]599[^0-9] > test1.txt")
--os.execute("cat log/DB/TRACE20140622 |grep var |grep [^0-9]599[^0-9] |grep \"\\[1[0-9]\" > test1.txt")  --需要修改cat语句已限制时间段
os.execute("awk '{print $11,$12,$13,$14}' test1.txt > test2.txt ")  --产生文件的格式为(XXXX, XXXX, XXXX)]
file = io.open("test2.txt","r")
local res = {}
local result = {}
--获得此时间段 各玩家的积分变化情况
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
            result[str1]= 100    --结果增量设置为0
        end

        --条件刷分情况 10分的递进刷分，此时间内全部还原(处理刷分) ,问题(只要是10分递进的加分全部清楚)
        --print(str1..tonumber(str2))
        if  tonumber(str2) - res[str1] == 10 then 
            result[str1] = result[str1] - 10     --结果：削减的积分（10分，若无条件削减，参见下文增加）
        end

        --    --此判断开放：表示该时间段积分重新增加（处理积分意外清零的情况）
        --    if  tonumber(str2) > res[str1]  then 
        --        result[str1] = result[str1] + tonumber(str2) - res[str1]  --结果：增加积分（无条件增加）
        --    end
        --
        res[str1] = tonumber(str2)
    else
        print(str3 .. "error")
    end
end
file:close()

--写入sql语句完成数据还原
fileOpen = io.open("sql/updates/Object_AriBook_Bug.sql","w")
for key,value in pairs(result) do
    local option ={"+","-"}   --结果为正，表示增加  结果为负，表示减少
    local opType = 1;
    if result[key] < 0 then 
        opType = 2
        result[key] = -result[key]
    end
    local strs = string.format("UPDATE var set data = data %s %d where id = 599 and playerId = %u and data >= %d;",option[opType],result[key],tonumber(key),result[key]*(opType-1))   --只影响总积分 
    print(strs)   --用于查看sql语句
    fileOpen:write(strs);
    fileOpen:write("\n")
end
fileOpen:close()
