--����Ľ�������
function Task_Accept_00010106()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10106) or task:HasCompletedTask(10106) or task:HasSubmitedTask(10106) then
		return false;
	end
	if not task:HasSubmitedTask(10105) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(10106) or task:HasCompletedTask(10106) or task:HasSubmitedTask(10106) then
		return false;
	end
	if not task:HasSubmitedTask(10105) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10106) == npcId and Task_Accept_00010106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "力量的试炼";
	elseif task:GetTaskSubmitNpc(10106) == npcId then
		if Task_Submit_00010106() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10106
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "力量的试炼";
		elseif task:HasAcceptedTask(10106) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10106
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "力量的试炼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我一猜就是那些最近混进天庭的鬼鬼祟祟的狼鬼族密探，他们一定是有什么阴谋，虽然你是新来到天族的勇士，可是我相信你有足够的勇气和力量将他们打败，不能放任他们在我们的地盘胡作非为。";
	action.m_ActionMsg = "我也这样认为。";
	return action;
end

function Task_00010106_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "拿起你的武器，去城郊给他们点教训吧，让这些邪恶的狼鬼族知道你的厉害。如果你能成功的打败他们，你的勇气将会获得嘉奖。";
	action.m_ActionMsg = "我一定会打败他们。";
	return action;
end

function Task_00010106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "干的漂亮"..GetPlayerName(GetPlayer()).."，这些狼鬼密探确实需要有人给他们点教训了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010106_step_table = {
		[1] = Task_00010106_step_01,
		[2] = Task_00010106_step_02,
		[10] = Task_00010106_step_10,
		};

function Task_00010106_step(step)
	if Task_00010106_step_table[step] ~= nil then
		return Task_00010106_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010106() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10106) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00010106_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8900,1,1) + package:GetItemUsedGrids(6,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10106) then
		return false;
	end

	if IsEquipTypeId(8900) then
		for k = 1, 1 do
			package:AddEquip(8900, 1);
		end
	else 
		package:AddItem(8900,1,1);
	end
	if IsEquipTypeId(6) then
		for k = 1, 1 do
			package:AddEquip(6, 1);
		end
	else 
		package:AddItem(6,1,1);
	end

	player:AddExp(800);
	player:getCoin(750);
	return true;
end

--��������
function Task_00010106_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10106);
end
