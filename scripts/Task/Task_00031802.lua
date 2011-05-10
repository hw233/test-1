--����Ľ�������
function Task_Accept_00031802()
	local player = GetPlayer();
	if player:GetLev() < 69 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31802) or task:HasCompletedTask(31802) or task:HasSubmitedTask(31802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31801) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031802()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 69 then
		return false;
	end
	if task:HasAcceptedTask(31802) or task:HasCompletedTask(31802) or task:HasSubmitedTask(31802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31801) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031802()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31802) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031802(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31802) == npcId and Task_Accept_00031802 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31802
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "寻找人质蓦然";
	elseif task:GetTaskSubmitNpc(31802) == npcId then
		if Task_Submit_00031802() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31802
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "寻找人质蓦然";
		elseif task:HasAcceptedTask(31802) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31802
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "寻找人质蓦然";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031802_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位勇士，好好的山海大陆的修炼旅程不去，跑到我这山谷中的慕士城来做什么？";
	action.m_ActionMsg = "我就是来找你这个镇邪派的叛徒，你挟持走了严中大人的女儿。";
	return action;
end

function Task_00031802_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我这么做也是自己的选择，夜摩盟愿意给我很多钱财，在这里我不用再像在镇邪派那里一样，过着穷苦的日子，什么事情都要自己动手，想最省钱的办法。我这个人就是爱财，这是不能改变的，至于严中的女儿，实在是因为严中一直对于我的背叛怀恨在心不肯放过我。";
	action.m_ActionMsg = "可这都是你自己活该啊。";
	return action;
end

function Task_00031802_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "人生真是无奈，不经意就会走上歧途。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031802_step_table = {
		[1] = Task_00031802_step_01,
		[2] = Task_00031802_step_02,
		[10] = Task_00031802_step_10,
		};

function Task_00031802_step(step)
	if Task_00031802_step_table[step] ~= nil then
		return Task_00031802_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031802_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031802() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31802) then
		return false;
	end
	task:AddTaskStep(31802);
	return true;
end



--�ύ����
function Task_00031802_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(846,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31802) then
		return false;
	end

	if IsEquipTypeId(846) then
		for k = 1, 1 do
			package:AddEquip(846, 1);
		end
	else 
		package:AddItem(846,1,1);
	end

	player:AddExp(220000);
	player:getCoin(204000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031802_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31802);
end
