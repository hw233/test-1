

--地图信息  0 不可通过的障碍   1 草地   2城镇    3森林     4山地
mapInfo =
{
     [1]={
	     [1] = {0 , 1,  1 , 1, 1 },
		 [2] = {1 , 1,  1 , 1, 1 },
		 [3] = {1 , 1,  1 , 1, 1 }
	 },
}



--地图上的阵营信息       //0属于障碍 不可通过  可视为中立阵营
mapCampInfo =
{
	[1]={
		[1] = {0,1,1,2,2},
		[2] = {1,1,0,2,2},                ---地图1的
		[3] = {0,1,1,0,2}
	}

}




---分别代表 在不可通过的障碍物  草地  城镇   森林   山地  上的行动力   与上述的地形相对应
rideWithLandform  = {0,2,1,1,1}


function GetMapInfo(id)
	return mapInfo[id]
end


function GetCampInfo(id)
	return mapCampInfo[id]
end

function GetRide(landform)
	return rideWithLandform[landform]
end

function GetInfo(id,x,y)
   return mapInfo[id][x][y]
end
