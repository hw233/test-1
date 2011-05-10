--����Ľ�������
function Task_Accept_00030703()
	local player = GetPlayer();
	if player:GetLev() < 47 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30703) or task:HasCompletedTask(30703) or task:HasSubmitedTask(30703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30702) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030703()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 47 then
		return false;
	end
	if task:HasAcceptedTask(30703) or task:HasCompletedTask(30703) or task:HasSubmitedTask(30703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30702) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030703()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30703) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030703(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30703) == npcId and Task_Accept_00030703 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30703
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "采集药材";
	elseif task:GetTaskAcceptNpc(30703) == npcId and task:HasAcceptedTask(30703) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30703
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "采集药材";
	elseif task:GetTaskSubmitNpc(30703) == npcId then
		if Task_Submit_00030703() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30703
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "采集药材";
		elseif task:HasAcceptedTask(30703) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30703
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "采集药材";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030703_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士现在感觉好多了吧，为你医好寒毒算你欠我一个人情，我正好想去这摩加河四周采集一些灵药，可是周围有许多妖魔怪兽，如果你能一直护送我，也算你还我的人情了，这样也不会耽误我太多时间。";
	action.m_ActionMsg = "当然没问题了，我感谢您还来不及呢。";
	return action;
end

function Task_00030703_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那么走吧，等我灵药采好了，你就可以离开摩加河了，去响石滩的黎明镇，向他们的首领祖言汇报一下你的境遇，他们会愿意结识你并给予你帮助的。";
	action.m_ActionMsg = "我需要更多的朋友和我并肩作战，速速出发吧。";
	return action;
end

function Task_00030703_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你到我们黎明镇来找我是想干什么？";
	action.m_ActionMsg = "";
	return action;
end

function Task_00030703_step_11()
	RunConveyAction(GetPlayer(),30703);
end

local Task_00030703_step_table = {
		[1] = Task_00030703_step_01,
		[2] = Task_00030703_step_02,
		[10] = Task_00030703_step_10,
		[11] = Task_00030703_step_11,
		};

function Task_00030703_step(step)
	if Task_00030703_step_table[step] ~= nil then
		return Task_00030703_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030703_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030703() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30703) then
		return false;
	end
	RunConveyAction(player,30703);
	return true;
end



--�ύ����
function Task_00030703_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8934,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30703) then
		return false;
	end

	if IsEquipTypeId(8934) then
		for k = 1, 1 do
			package:AddEquip(8934, 1);
		end
	else 
		package:AddItem(8934,1,1);
	end

	player:AddExp(44000);
	player:getCoin(31500);
	player:getTael(20);
	return true;
end

--��������
function Task_00030703_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30703);
end
