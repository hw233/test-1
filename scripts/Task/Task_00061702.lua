--����Ľ�������
function Task_Accept_00061702()
	local player = GetPlayer();
	if player:GetLev() < 63 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61702) or task:HasCompletedTask(61702) or task:HasSubmitedTask(61702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61701) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061702()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 63 then
		return false;
	end
	if task:HasAcceptedTask(61702) or task:HasCompletedTask(61702) or task:HasSubmitedTask(61702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61701) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061702()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61702) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061702(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61702) == npcId and Task_Accept_00061702 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61702
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "被偷走的钱";
	elseif task:GetTaskSubmitNpc(61702) == npcId then
		if Task_Submit_00061702() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61702
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "被偷走的钱";
		elseif task:HasAcceptedTask(61702) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61702
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "被偷走的钱";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061702_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你居然是为那无名醉鬼来讨债的？真是见鬼了，他每天醉醺醺的怎么找到你的啊勇士，再说了，我没还他钱也是因为我没有钱啊，我要是有钱怎么会不还呢。";
	action.m_ActionMsg = "那你的钱呢？";
	return action;
end

function Task_00061702_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "说起我的钱啊，太悲剧了，我好不容易攒了点钱，准备还债的，结果不远处的飓风部落冲过来一群夜摩密盗，一下把我的钱都抢走了，我也打不过他们，唉。";
	action.m_ActionMsg = "那我去帮你把钱抢回来，最好你没有说谎。";
	return action;
end

function Task_00061702_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哇勇士你太厉害了，还真的把我的钱抢回来了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061702_step_table = {
		[1] = Task_00061702_step_01,
		[2] = Task_00061702_step_02,
		[10] = Task_00061702_step_10,
		};

function Task_00061702_step(step)
	if Task_00061702_step_table[step] ~= nil then
		return Task_00061702_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061702_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061702() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61702) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15016,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15016, itemNum, 1);
		end
		task:AddTaskStep2(61702, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00061702_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15016,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61702) then
		return false;
	end

	package:DelItemAll(15016,1);

	player:AddExp(160000);
	player:getCoin(130000);
	return true;
end

--��������
function Task_00061702_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15016,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61702);
end
