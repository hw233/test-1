--����Ľ�������
function Task_Accept_00000026()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(26) or task:HasCompletedTask(26) or task:HasSubmitedTask(26) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000026()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(26) or task:HasCompletedTask(26) or task:HasSubmitedTask(26) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000026()
	if GetPlayer():GetTaskMgr():HasCompletedTask(26) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000026(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(26) == npcId and Task_Accept_00000026 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 26
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "异人收徒";
	elseif task:GetTaskSubmitNpc(26) == npcId then
		if Task_Submit_00000026() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 26
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "异人收徒";
		elseif task:HasAcceptedTask(26) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 26
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "异人收徒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000026_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "接下来就让你帮我跑跑腿了，和你一起的不是有个叫周淳的吗，虽然年过四十又不是童身，但别人说不好的自己偏偏就觉得好，我就是要收周淳做弟子，这是另外一枚补髓益元丹你去碧均庵送给他。";
	action.m_ActionMsg = "前辈真乃是风尘异人啊。";
	return action;
end

function Task_00000026_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是感谢"..GetPlayerName(GetPlayer()).."你来告诉我这个喜讯。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000026_step_table = {
		[1] = Task_00000026_step_01,
		[10] = Task_00000026_step_10,
		};

function Task_00000026_step(step)
	if Task_00000026_step_table[step] ~= nil then
		return Task_00000026_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000026_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000026() then
		return false;
	end
	local package = player:GetPackage();
	local reqGrids = 0;
	reqGrids = reqGrids + package:GetItemUsedGrids(800, 1, 1);
	if reqGrids > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2012, 0);
		return false;
	end
	if not task:AcceptTask(26) then
		return false;
	end
	package:AddItem(800, 1, 1);
	task:AddTaskStep(26);
	return true;
end



--�ύ����
function Task_00000026_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(26) then
		return false;
	end


	player:AddExp(6000);
	return true;
end

--��������
function Task_00000026_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItem(800, 1, 1);
	return GetPlayer():GetTaskMgr():AbandonTask(26);
end
