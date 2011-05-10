--����Ľ�������
function Task_Accept_00010405()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 23 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10405) or task:HasCompletedTask(10405) or task:HasSubmitedTask(10405) then
		return false;
	end
	if not task:HasSubmitedTask(10404) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010405()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 23 then
		return false;
	end
	if task:HasAcceptedTask(10405) or task:HasCompletedTask(10405) or task:HasSubmitedTask(10405) then
		return false;
	end
	if not task:HasSubmitedTask(10404) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010405()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10405) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010405(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10405) == npcId and Task_Accept_00010405 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10405
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "前往观音山";
	elseif task:GetTaskSubmitNpc(10405) == npcId then
		if Task_Submit_00010405() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10405
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "前往观音山";
		elseif task:HasAcceptedTask(10405) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10405
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "前往观音山";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010405_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "历史和现实都是残酷而充满仇恨的，只有更加强大起来，才能避免悲剧，勇士希望你能明白，天族需要你的加入并为之奉献。";
	action.m_ActionMsg = "我明白，我尽最大的努力。";
	return action;
end

function Task_00010405_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想你可以离开这里去观音山了，圣严正在观音殿里等你，这块石碑你带去交给他，他看到了会明白我的意思。";
	action.m_ActionMsg = "看来我又面临新的任务了。";
	return action;
end

function Task_00010405_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是明镜让你带来的石碑，看了形式的确很严峻了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010405_step_table = {
		[1] = Task_00010405_step_01,
		[2] = Task_00010405_step_02,
		[10] = Task_00010405_step_10,
		};

function Task_00010405_step(step)
	if Task_00010405_step_table[step] ~= nil then
		return Task_00010405_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010405_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010405() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15026, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10405) then
		return false;
	end
	task:AddTaskStep(10405);
	return true;
end



--�ύ����
function Task_00010405_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15026,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8905,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10405) then
		return false;
	end

	if IsEquipTypeId(8905) then
		for k = 1, 1 do
			package:AddEquip(8905, 1);
		end
	else 
		package:AddItem(8905,1,1);
	end
	package:DelItemAll(15026,1);

	player:AddExp(2200);
	player:getCoin(4300);
	player:getTael(10);
	return true;
end

--��������
function Task_00010405_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15026,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10405);
end
