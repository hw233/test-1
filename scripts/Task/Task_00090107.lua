--����Ľ�������
function Task_Accept_00090107()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90107) or task:HasCompletedTask(90107) or task:HasSubmitedTask(90107) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090107()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90107) or task:HasCompletedTask(90107) or task:HasSubmitedTask(90107) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090107()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90107) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090107(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90107) == npcId and Task_Accept_00090107 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90107
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "可怕的梦境";
	elseif task:GetTaskSubmitNpc(90107) == npcId then
		if Task_Submit_00090107() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90107
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "可怕的梦境";
		elseif task:HasAcceptedTask(90107) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90107
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "可怕的梦境";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090107_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你是谁？我认识你吗？";
	action.m_ActionMsg = "我是代巫师来看你的，最近可好？";
	return action;
end

function Task_00090107_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "哦，谢谢你。我不太好，最近总做噩梦，梦见海底有一群流血的骷髅，太恐怖了。";
	action.m_ActionMsg = "流血的骷髅？难倒是沙波海底古窟的残血骷髅在作怪？（沉思中）";
	return action;
end

function Task_00090107_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "海底？你能帮我去看看吗？";
	action.m_ActionMsg = "小事一桩，你等我。";
	return action;
end

function Task_00090107_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真的是那些怪物在害我吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090107_step_table = {
		[1] = Task_00090107_step_01,
		[2] = Task_00090107_step_02,
		[3] = Task_00090107_step_03,
		[10] = Task_00090107_step_10,
		};

function Task_00090107_step(step)
	if Task_00090107_step_table[step] ~= nil then
		return Task_00090107_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090107_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090107() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90107) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090107_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90107) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--��������
function Task_00090107_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90107);
end
