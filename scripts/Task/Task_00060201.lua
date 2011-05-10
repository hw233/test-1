--����Ľ�������
function Task_Accept_00060201()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 31 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60201) or task:HasCompletedTask(60201) or task:HasSubmitedTask(60201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 31 then
		return false;
	end
	if task:HasAcceptedTask(60201) or task:HasCompletedTask(60201) or task:HasSubmitedTask(60201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60201) == npcId and Task_Accept_00060201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "神秘信封";
	elseif task:GetTaskSubmitNpc(60201) == npcId then
		if Task_Submit_00060201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "神秘信封";
		elseif task:HasAcceptedTask(60201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "神秘信封";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "魔族前线哨所的传送门，可以通往地界。你们现在要集结部队，由巨石堆的巨型狼人来接应，集体攻通过传送门以后，从鬼蜮山打到什刹海，以后地界就是我们的了。";
	action.m_ActionMsg = "居然在密谋占领我们的地界！";
	return action;
end

function Task_00060201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，你说这个是狼鬼族的进攻计划？ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060201_step_table = {
		[1] = Task_00060201_step_01,
		[10] = Task_00060201_step_10,
		};

function Task_00060201_step(step)
	if Task_00060201_step_table[step] ~= nil then
		return Task_00060201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60201) then
		return false;
	end
	task:AddTaskStep(60201);
	return true;
end



--�ύ����
function Task_00060201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15001,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60201) then
		return false;
	end

	package:DelItemAll(15001,1);

	player:AddExp(4000);
	player:getCoin(5600);
	return true;
end

--��������
function Task_00060201_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15001,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60201);
end
