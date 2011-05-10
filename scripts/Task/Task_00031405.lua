--����Ľ�������
function Task_Accept_00031405()
	local player = GetPlayer();
	if player:GetLev() < 64 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31405) or task:HasCompletedTask(31405) or task:HasSubmitedTask(31405) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31404) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31404) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031405()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 64 then
		return false;
	end
	if task:HasAcceptedTask(31405) or task:HasCompletedTask(31405) or task:HasSubmitedTask(31405) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31404) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31404) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031405()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31405) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031405(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31405) == npcId and Task_Accept_00031405 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31405
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "多年对峙";
	elseif task:GetTaskSubmitNpc(31405) == npcId then
		if Task_Submit_00031405() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31405
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "多年对峙";
		elseif task:HasAcceptedTask(31405) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31405
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "多年对峙";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031405_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我已经多次听说了你在山海大陆的英勇事迹，可是你来到这片绵延起伏的南岭山脉时间还不长，也许还不知道夜摩盟都做了些什么。夜摩盟的罗阅王本来只是一个力量弱小的狼人，在加入夜摩盟以后，很快升为军备首领，势力迅速扩张，并且借助南岭山脉的复杂地形，虽然到处肆虐，却很难被捉到，我作为东山派的敢死将军已经在这里与他斗争多年了。\n";
	action.m_ActionMsg = "请继续告诉我吧……";
	return action;
end

function Task_00031405_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "最近罗阅王不知道又在闭关研究什么，他将自己锁在碎心塔里很久不出来，并布置了大量的力量把守着碎心塔的钥匙，我想他一定有什么阴谋，无论如何一定要在他得逞之前阻止他。\n";
	action.m_ActionMsg = "原来是这样。";
	return action;
end

function Task_00031405_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "打败罗阅王的时候到了，一定不能让他的阴谋得逞。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031405_step_table = {
		[1] = Task_00031405_step_01,
		[2] = Task_00031405_step_02,
		[10] = Task_00031405_step_10,
		};

function Task_00031405_step(step)
	if Task_00031405_step_table[step] ~= nil then
		return Task_00031405_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031405_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031405() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31405) then
		return false;
	end
	task:AddTaskStep(31405);
	return true;
end



--�ύ����
function Task_00031405_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8906,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31405) then
		return false;
	end

	if IsEquipTypeId(8906) then
		for k = 1, 1 do
			package:AddEquip(8906, 1);
		end
	else 
		package:AddItem(8906,1,1);
	end

	player:AddExp(170000);
	player:getCoin(145000);
	return true;
end

--��������
function Task_00031405_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31405);
end
