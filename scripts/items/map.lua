

--��ͼ��Ϣ  0 ����ͨ�����ϰ�   1 �ݵ�   2����    3ɭ��     4ɽ��
mapInfo =
{
     [1]={
	     [1] = {0 , 1,  1 , 1, 1 },
		 [2] = {1 , 1,  1 , 1, 1 },
		 [3] = {1 , 1,  1 , 1, 1 }
	 },
}



--��ͼ�ϵ���Ӫ��Ϣ       //0�����ϰ� ����ͨ��  ����Ϊ������Ӫ
mapCampInfo =
{
	[1]={
		[1] = {0,1,1,2,2},
		[2] = {1,1,0,2,2},                ---��ͼ1��
		[3] = {0,1,1,0,2}
	}

}




---�ֱ���� �ڲ���ͨ�����ϰ���  �ݵ�  ����   ɭ��   ɽ��  �ϵ��ж���   �������ĵ������Ӧ
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
