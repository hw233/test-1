--����Ľ�������
function Task_Accept_00090116()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90116) or task:HasCompletedTask(90116) or task:HasSubmitedTask(90116) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090116()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90116) or task:HasCompletedTask(90116) or task:HasSubmitedTask(90116) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090116()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90116) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090116(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90116) == npcId and Task_Accept_00090116 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90116
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "清除亡魂";
	elseif task:GetTaskSubmitNpc(90116) == npcId then
		if Task_Submit_00090116() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90116
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "清除亡魂";
		elseif task:HasAcceptedTask(90116) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90116
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "清除亡魂";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090116_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "好酒不怕巷子深，长者的一番好意我心领了。最近那些复仇怨灵总是吵闹不休，长者很是烦恼。你去将那些怨灵杀掉，让长者清静清静，完事儿后去告诉长者。";
	action.m_ActionMsg = "看来现在哪儿都是不得安宁的。";
	return action;
end

function Task_00090116_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老夫终于可以清静清静了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090116_step_table = {
		[1] = Task_00090116_step_01,
		[10] = Task_00090116_step_10,
		};

function Task_00090116_step(step)
	if Task_00090116_step_table[step] ~= nil then
		return Task_00090116_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090116_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090116() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90116) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090116_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90116) then
		return false;
	end


	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--��������
function Task_00090116_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90116);
end
