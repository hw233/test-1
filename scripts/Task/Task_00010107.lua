--����Ľ�������
function Task_Accept_00010107()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10107) or task:HasCompletedTask(10107) or task:HasSubmitedTask(10107) then
		return false;
	end
	if not task:HasSubmitedTask(10106) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010107()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(10107) or task:HasCompletedTask(10107) or task:HasSubmitedTask(10107) then
		return false;
	end
	if not task:HasSubmitedTask(10106) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010107()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10107) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010107(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10107) == npcId and Task_Accept_00010107 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10107
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "成长的脚步";
	elseif task:GetTaskSubmitNpc(10107) == npcId then
		if Task_Submit_00010107() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10107
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "成长的脚步";
		elseif task:HasAcceptedTask(10107) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10107
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "成长的脚步";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010107_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "不要忘记你是骄傲的天族勇士，不要忘记你背负对魔族的仇恨，对力量的追求，对邪恶的憎恶。";
	action.m_ActionMsg = "我不会忘记。";
	return action;
end

function Task_00010107_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在你需要去南边瞻部峰的武都镇，找到我最好的朋友摩伽天，他是我们天族的国师，会给你的成长更好的指导，顺便带去这坛美酒给他，并告诉他我对他的想念。";
	action.m_ActionMsg = "是您的好朋友啊，那我这就去找他。";
	return action;
end

function Task_00010107_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "啊原来是忉利天送来的美酒，好久没有喝到了。还好这个家伙这么忙居然还想着我。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010107_step_table = {
		[1] = Task_00010107_step_01,
		[2] = Task_00010107_step_02,
		[10] = Task_00010107_step_10,
		};

function Task_00010107_step(step)
	if Task_00010107_step_table[step] ~= nil then
		return Task_00010107_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010107_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010107() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15019, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10107) then
		return false;
	end
	task:AddTaskStep(10107);
	return true;
end



--�ύ����
function Task_00010107_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15019,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(7,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10107) then
		return false;
	end

	if IsEquipTypeId(7) then
		for k = 1, 1 do
			package:AddEquip(7, 1);
		end
	else 
		package:AddItem(7,1,1);
	end
	package:DelItemAll(15019,1);

	player:AddExp(800);
	player:getCoin(800);
	player:getTael(5);
	return true;
end

--��������
function Task_00010107_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15019,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10107);
end
