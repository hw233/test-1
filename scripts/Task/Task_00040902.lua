--����Ľ�������
function Task_Accept_00040902()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40902) or task:HasCompletedTask(40902) or task:HasSubmitedTask(40902) then
		return false;
	end
	if not task:HasSubmitedTask(40901) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040902()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(40902) or task:HasCompletedTask(40902) or task:HasSubmitedTask(40902) then
		return false;
	end
	if not task:HasSubmitedTask(40901) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040902()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40902) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040902(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40902) == npcId and Task_Accept_00040902 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40902
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "战争洗礼";
	elseif task:GetTaskSubmitNpc(40902) == npcId then
		if Task_Submit_00040902() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40902
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "战争洗礼";
		elseif task:HasAcceptedTask(40902) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40902
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "战争洗礼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040902_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你确定要接受这战争的洗礼了？";
	action.m_ActionMsg = "大德法师都这样说了，我只好来了。";
	return action;
end

function Task_00040902_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那么你只需在我这观音殿里，使用我为你准备的洗礼符，便可脑体净透，充满勇气面对未来，不再被红尘凡事所累。完成以后便回到大德法师那里去吧。";
	action.m_ActionMsg = "洗就洗吧。";
	return action;
end

function Task_00040902_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "接受了洗礼的勇士，浑身散发的气息已经不一样了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040902_step_table = {
		[1] = Task_00040902_step_01,
		[2] = Task_00040902_step_02,
		[10] = Task_00040902_step_10,
		};

function Task_00040902_step(step)
	if Task_00040902_step_table[step] ~= nil then
		return Task_00040902_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040902_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040902() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15054, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(40902) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040902_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15054,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(40902) then
		return false;
	end

	package:DelItemAll(15054,1);

	player:AddExp(4500);
	player:getCoin(4700);
	return true;
end

--��������
function Task_00040902_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15054,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40902);
end
