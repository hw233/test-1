--����Ľ�������
function Task_Accept_00000108()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(108) or task:HasCompletedTask(108) or task:HasSubmitedTask(108) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(107) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(107) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000108()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(108) or task:HasCompletedTask(108) or task:HasSubmitedTask(108) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(107) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(107) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000108()
	if GetPlayer():GetTaskMgr():HasCompletedTask(108) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000108(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(108) == npcId and Task_Accept_00000108 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 108
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "百毒金蚕";
	elseif task:GetTaskSubmitNpc(108) == npcId then
		if Task_Submit_00000108() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 108
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "百毒金蚕";
		elseif task:HasAcceptedTask(108) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 108
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "百毒金蚕";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000108_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "绿袍老祖魔焰滔天，邪功高强，不过他最厉害的法宝确实用阴风洞特产的金蚕炼制的百毒金蚕蛊，无论何等剑侠人物，被那金蚕一咬，都难免命丧黄泉。";
	action.m_ActionMsg = "这法宝听起来似乎很厉害啊……";
	return action;
end

function Task_00000108_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "百毒金蚕蛊固然威力绝伦，可是太过凶毒，有干天和，之前慈云寺一战，绿袍被极乐童子前辈破了邪功，金蚕也死伤大半。如今绿袍又在金峰崖训练金蚕，须得尽早将这些金蚕除掉，不然等绿袍炼制成功，除魔之计恐怕又增变数。";
	action.m_ActionMsg = "掌教放心，弟子这就去金峰崖将这些金蚕消灭。";
	return action;
end

function Task_00000108_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "没了这些金蚕，绿袍就等于没牙的老虎。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000108_step_table = {
		[1] = Task_00000108_step_01,
		[2] = Task_00000108_step_02,
		[10] = Task_00000108_step_10,
		};

function Task_00000108_step(step)
	if Task_00000108_step_table[step] ~= nil then
		return Task_00000108_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000108_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000108() then
		return false;
	end
	if not task:AcceptTask(108) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000108_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(108) then
		return false;
	end


	player:AddExp(35000);
	return true;
end

--��������
function Task_00000108_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(108);
end
