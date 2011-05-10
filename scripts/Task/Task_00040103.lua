--����Ľ�������
function Task_Accept_00040103()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40103) or task:HasCompletedTask(40103) or task:HasSubmitedTask(40103) then
		return false;
	end
	if not task:HasSubmitedTask(40102) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(40103) or task:HasCompletedTask(40103) or task:HasSubmitedTask(40103) then
		return false;
	end
	if not task:HasSubmitedTask(40102) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40103) == npcId and Task_Accept_00040103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "猎杀冰火狐";
	elseif task:GetTaskSubmitNpc(40103) == npcId then
		if Task_Submit_00040103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "猎杀冰火狐";
		elseif task:HasAcceptedTask(40103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "猎杀冰火狐";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "不然这样，女人没有不喜欢高级衣服的，你代我去做一件珍贵的狐皮罩衣送给悦意，好安慰他一下。在胜神洲有一片神秘的无烟谷，只有我的允许才可以进入打猎，杀掉那里的冰火狐，将狐皮带回来。";
	action.m_ActionMsg = "您真的很有办法。";
	return action;
end

function Task_00040103_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "冰火狐的皮毛是很珍贵的，但是只有一个人能将它处理成华丽的罩衣，去名人堂找郭嘉吧，跟他说清楚来意就可以了。";
	action.m_ActionMsg = "明白了。";
	return action;
end

function Task_00040103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你怎么会去弄这漂亮的狐皮？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040103_step_table = {
		[1] = Task_00040103_step_01,
		[2] = Task_00040103_step_02,
		[10] = Task_00040103_step_10,
		};

function Task_00040103_step(step)
	if Task_00040103_step_table[step] ~= nil then
		return Task_00040103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40103) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15055,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15055, itemNum, 1);
		end
		task:AddTaskStep2(40103, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00040103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15055,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(40103) then
		return false;
	end

	package:DelItemAll(15055,1);

	player:AddExp(800);
	player:getCoin(600);
	return true;
end

--��������
function Task_00040103_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15055,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40103);
end
