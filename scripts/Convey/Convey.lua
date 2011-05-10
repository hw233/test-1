require("Convey/ConveyTable");

--used for the task editor
function GetConveyTable()
	return Convey_Table;
end
	
function RunConvey(taskId)
	local pathTable = Convey_Table[taskId];
	if pathTable == nil then
		return {};
	end
	return pathTable;
end

function GetConveyMonster(taskId, spot)
	if Convey_Table[taskId] == nil then
		return 0;
	end
	local pathTable = Convey_Table[taskId];
	local size = table.getn(pathTable);
	if size < 3 then
		return 0;
	end
	local i = 1;
	while i <= (size-1)/2 do
		if pathTable[i*2] == spot then
			return pathTable[i*2+1];
		end
		i = i + 1;
	end;
	return 0;
end

--���»����������, ����ʧ�ܣ� ȫ�������ǰ�Ľ���
function UpdateConveyStep(taskId, spot, succ)
	--��ȡ����ݵ��Ӧ���������ͽ׶�
	local player = GetPlayer();
	local taskMgr = player:GetTaskMgr();
	if not succ then
		--ʧ�ܣ� ȫ���
		taskMgr:ResetTaskStep(taskId);
		return false;
	end
	local pathTable = Convey_Table[taskId];
	local size = table.getn(pathTable);
	if size < 3 then
		return false;
	end
	local i = 1;
	while i <= (size-1)/2 do
		if pathTable[i*2] == spot then
			break;
		end
		i = i + 1;
	end;
	if i > (size-1)/2 then
		return false;
	end
	--��������ǰ��Ĳ����Ƿ������
	if not taskMgr:CheckPreTaskStep(taskId, i) then
		return false;
	end
	taskMgr:AddTaskStep2(taskId, i, 1);
	return true;
end
