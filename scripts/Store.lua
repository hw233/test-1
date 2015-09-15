---
--
--  just for store by qsj  15/9/2
--
--
--
--
local storeItems = {

    --player level 1-30 
    [30] = {
        --table 1 
        [1] = {

            --type 1   
            [1] = { 

                --itemId  limitcount price  coinType
                { 1 , 1 , 3000 , 20001},
                { 2 , 1 , 3000 , 20002},
                { 3 , 1 , 3000 , 20001 }
            },
            [2] = { 
                { 41003 , 1 , 10 , 20002 },
                { 41004 , 1 , 10 , 20002 },
                { 41005 , 1 , 10 , 20002 },
                { 41006 , 1 , 10 , 20002 },
                { 41007 , 1 , 10 , 20002 },
                { 41008 , 1 , 10 , 20002 },
                { 41009 , 1 , 10 , 20002 },
                { 41011 , 1 , 10 , 20002 },
                { 41012 , 1 , 10 , 20002 },
                { 41013 , 1 , 10 , 20002 },
                { 41014 , 1 , 10 , 20002 },
                { 41015 , 1 , 10 , 20002 },
                { 41016 , 1 , 10 , 20002 },
                { 41017 , 1 , 10 , 20002 },
                { 41018 , 1 , 10 , 20002 },
                { 41019 , 1 , 10 , 20002 },
            },
        },
       [2] = {
           [1] = {
               { 501 , 100 , 100 , 20003 },
               { 502 , 100 , 100 , 20003 },
               { 503 , 100 , 100 , 20003 },
               { 504 , 100 , 100 , 20003 }
           },
           [2] = {
               { 41002 , 100 , 100 , 20003 },
               { 41010 , 1 , 500 , 20003   } 
           }
       }
    },
    [60] = { 
        [1] = {
            [1] = {
                { 4 , 1 , 3000 , 20001 },
                { 5 , 1 , 3000 , 20001 },
                { 6 , 1 , 3000 , 20001 }
            },
            [2] = {
                { 41003 , 1 , 10 , 20002 },
                { 41004 , 1 , 10 , 20002 },
                { 41005 , 1 , 10 , 20002 },
                { 41006 , 1 , 10 , 20002 },
                { 41007 , 1 , 10 , 20002 },
                { 41008 , 1 , 10 , 20002 },
                { 41009 , 1 , 10 , 20002 },
                { 41011 , 1 , 10 , 20002 },
                { 41012 , 1 , 10 , 20002 },
                { 41013 , 1 , 10 , 20002 },
                { 41014 , 1 , 10 , 20002 },
                { 41015 , 1 , 10 , 20002 },
                { 41016 , 1 , 10 , 20002 },
                { 41017 , 1 , 10 , 20002 },
                { 41018 , 1 , 10 , 20002 },
                { 41019 , 1 , 10 , 20002 }
            }
        },
        [2] = {
            [1] = {
                { 501 , 100 , 100 , 20003 },
                { 502 , 100 , 100 , 20003 },
                { 503 , 100 , 100 , 20003 },
                { 504 , 100 , 100 , 20003 }

            },
            [2] = {
                { 41002 , 1 , 500 , 20003 },
                { 41010 , 1 , 500 , 20003 }
            }
        }

    }
}


local itemCount = {
    [30] = { 
        [1] = { 4 , 2 },
        [2] = { 4 , 2 }
    },
    [60] = {
        [1] = { 4 , 2 },
        [2] = { 4 , 2 }
    }
}


local itemProb = {
    [30] = {
        [1] = {
            [1] = {1000, 2000, 10000},
            [2] = { 1000, 1500, 2000, 3000, 4000, 5000, 6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 9900, 10000}

        },
        [2] = {
            [1] = { 2000, 5000, 8000, 10000},
            [2] = { 2000, 10000}

        },

    },
    [60] = {
        [1] = {
            [1] = { 1000, 3000, 10000},
            [2] = { 1000, 1200, 1300, 1500, 2000, 2500, 3000, 3500, 4000, 5000, 5800, 6200, 6500, 7000, 8000, 10000} 
        },
        [2] = {
            [1] = { 2000, 4000, 8000, 10000},
            [2] = { 5000, 10000}
        }

    },
}



---index  table index 
function RandomItems(key, index )

    local count = itemCount[key][index]
    local items = storeItems[key][index] 
    local prob = itemProb[key][index]

    local randItems = {}

    for i = 1, #count do    --第index个
        local num = count[i]    --需要的各种类型的物品的数量
        local pro = prob[i]     --以及各种类型的物品出现的概率

        for j = 1 , num do
            local p = math.random(1, 10000)
            local flag = 0
            for k = 1, #pro do
                if p <= pro[k] then
                    flag = k
                    break
                end
            end
            table.insert(randItems,items[i][flag])
        end
    end

    return randItems
end


function GetRandItems(key)
    local pageSize = #storeItems[key]
    local items = {}

    for i=1, pageSize do
        local temp = {}
        table.insert(temp,i)
        local randItems = RandomItems(key,i)
        table.insert(temp,randItems)
        table.insert(items,temp)
    end

    return items;
end


function loadItems(player)
    if player == nil then
        return false
    end
    local store = player:GetStoreA()
    if store == nil then 
        return false
    end
    local level = player:GetLevel()

    local key = 0 
    if level < 30 then 
        key = 30 
    elseif level < 60 then 
        key = 60
    else
        key = 30
    end

    local pageItems = GetRandItems(key)
    for i=1, #pageItems do
       local pageId = pageItems[i][1] 
       local items  = pageItems[i][2]

       for j=1,#items do
           store:Add(pageId,items[j][1],items[j][2],items[j][3],items[j][4])
       end
    end
    return true;
end


function GetRandPageItems(key,pageId)
    local items = {}

    local temp = {}
    table.insert(temp,i)
    local randItems = RandomItems(key,pageId)
    table.insert(temp,randItems)
    table.insert(items,temp)

    return items
end


---加载某一固定页面
function loadPageItems(player,pageId)
    if player == nil then 
        return false
    end
    local level = player:GetLevel()

    local key = 0 
    if level < 30 then 
        key = 30 
    elseif level < 60 then 
        key = 60
    else
        key = 30
    end
    print ( " the key is " .. key )
    store:clear()
    local items = GetRandPageItems(key,pageId)
    local store = player:GetStoreA()
    if store == nil then
        return false;
    end
    for j=1,#items do
        store:Add(pageId,items[j][1],items[j][2],items[j][3],items[j][4])
    end
    return true;
end

