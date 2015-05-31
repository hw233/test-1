--map info :  0  uncross  1 grass 2 town 3 forest 4 hill
mapInfo =
{
     [1]={
	     [1] = {0 , 1,  2 , 1, 1 },
		 [2] = {1 , 1,  3 , 1, 1 },
		 [3] = {0 , 1,  1 , 4, 1 }
	 },
}


--map camp info : 0 uncross 1 camp one 2 camp two
mapCampInfo =
{
	[1]={
		[1] = {0,1,1,2,2},
		[2] = {1,1,0,2,2},               --corresponding with mapinfo
		[3] = {0,1,1,0,2}
	}

}

--ride sub with different soldier and diff landform 
rideWithLandform  = {
    [1] = {1,2,1,3},   --walker 
    [2] = {1,2,2,3},   --hoser  in  grass town forest hill ridesub 
    [3] = {1,2,1,3}    --shooter
}

soldierBaseData={
    [1] = { 1, 2 }, --waller 
    [2] = { 1, 3 }, --hoser  attackRange movePower
    [3] = { 2, 2 }  --shooter
}


function GetMap()
    return mapInfo
end

function GetCamp()
    return mapCampInfo
end

function GetMapInfo(id)
	return mapInfo[id]
end


function GetCampInfo(id)
	return mapCampInfo[id]
end

function GetInfo(id,x,y)
   return mapInfo[id][x][y]
end

function GetMovePower(stype)
    return soldierBaseData[stype][2]
end

function GetAttackRange(stype)
    return soldierBaseData[stype][1]
end

function GetRideSub(stype,lanform)
    return rideWithLandform[stype][lanform]
end
