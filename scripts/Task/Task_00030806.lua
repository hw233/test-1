--����Ľ�������
function Task_Accept_00030806()
	local player = GetPlayer();
	if player:GetLev() < 52 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30806) or task:HasCompletedTask(30806) or task:HasSubmitedTask(30806) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30805) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30805) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030806()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 52 then
		return false;
	end
	if task:HasAcceptedTask(30806) or task:HasCompletedTask(30806) or task:HasSubmitedTask(30806) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30805) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30805) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030806()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30806) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030806(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30806) == npcId and Task_Accept_00030806 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30806
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "大战须余多";
	elseif task:GetTaskSubmitNpc(30806) == npcId then
		if Task_Submit_00030806() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30806
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "大战须余多";
		elseif task:HasAcceptedTask(30806) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30806
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "大战须余多";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030806_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."感谢你带来了飞鸣为我打造的装备，让我重又找回当年驰骋沙场的感觉，我要挥起我的武器，让那些曾经让祖言的家族陷入痛苦的夜摩盟尝尝痛苦的滋味。";
	action.m_ActionMsg = "您真是太有气势了。";
	return action;
end

function Task_00030806_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻人你别急，勇士的气势要在战斗中才能体现出来，根据最新的情报，夜摩盟的主将须臾多和他的部下正躲在无声塔里，如果你够勇敢，就跟我一起来吧，打败他们，证明自己的勇敢和气势。";
	action.m_ActionMsg = "太好了，我也正等着这样的机会呢。";
	return action;
end

function Task_00030806_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哈哈勇士，我们第一次合作就这么成功，希望以后有更多的机会一起作战。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030806_step_table = {
		[1] = Task_00030806_step_01,
		[2] = Task_00030806_step_02,
		[10] = Task_00030806_step_10,
		};

function Task_00030806_step(step)
	if Task_00030806_step_table[step] ~= nil then
		return Task_00030806_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030806_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030806() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30806) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030806_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8906,2,1) + package:GetItemUsedGrids(1303,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30806) then
		return false;
	end

	if IsEquipTypeId(8906) then
		for k = 1, 2 do
			package:AddEquip(8906, 1);
		end
	else 
		package:AddItem(8906,2,1);
	end
	if IsEquipTypeId(1303) then
		for k = 1, 1 do
			package:AddEquip(1303, 1);
		end
	else 
		package:AddItem(1303,1,1);
	end

	player:AddExp(64000);
	player:getCoin(53000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030806_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30806);
end
