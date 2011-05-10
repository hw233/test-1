--����Ľ�������
function Task_Accept_00031406()
	local player = GetPlayer();
	if player:GetLev() < 64 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31406) or task:HasCompletedTask(31406) or task:HasSubmitedTask(31406) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31405) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31405) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031406()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 64 then
		return false;
	end
	if task:HasAcceptedTask(31406) or task:HasCompletedTask(31406) or task:HasSubmitedTask(31406) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31405) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31405) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031406()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31406) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031406(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31406) == npcId and Task_Accept_00031406 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31406
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "直击罗阅王";
	elseif task:GetTaskSubmitNpc(31406) == npcId then
		if Task_Submit_00031406() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31406
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "直击罗阅王";
		elseif task:HasAcceptedTask(31406) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31406
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "直击罗阅王";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031406_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士现在既然你已经来到了这里，并且为我们获得了碎心塔的钥匙，我想我们来击败罗阅王的时机已经成熟，只要你做好充分的战斗准备，我将率领我的部下全力协力你。";
	action.m_ActionMsg = "没问题的。";
	return action;
end

function Task_00031406_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "罗阅王是夜摩盟中实力不可小觑的一个王，当你做好万全的战斗准备之后，就请随我来到碎心塔，我会为你用你带来的钥匙打开碎心塔，罗阅王的阴谋就会暴露，用我们的力量齐心协力打败他吧。";
	action.m_ActionMsg = "冲啊，让他尝尝我的厉害。";
	return action;
end

function Task_00031406_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你的名字会在我们对抗夜摩盟的历程中永远铭记。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031406_step_table = {
		[1] = Task_00031406_step_01,
		[2] = Task_00031406_step_02,
		[10] = Task_00031406_step_10,
		};

function Task_00031406_step(step)
	if Task_00031406_step_table[step] ~= nil then
		return Task_00031406_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031406_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031406() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31406) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031406_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1304,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31406) then
		return false;
	end

	if IsEquipTypeId(1304) then
		for k = 1, 1 do
			package:AddEquip(1304, 1);
		end
	else 
		package:AddItem(1304,1,1);
	end

	player:AddExp(170000);
	player:getCoin(145000);
	player:getTael(60);
	return true;
end

--��������
function Task_00031406_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31406);
end
