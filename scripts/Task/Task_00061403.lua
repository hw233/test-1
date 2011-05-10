--����Ľ�������
function Task_Accept_00061403()
	local player = GetPlayer();
	if player:GetLev() < 56 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61403) or task:HasCompletedTask(61403) or task:HasSubmitedTask(61403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61402) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 56 then
		return false;
	end
	if task:HasAcceptedTask(61403) or task:HasCompletedTask(61403) or task:HasSubmitedTask(61403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61402) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61403) == npcId and Task_Accept_00061403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "逆风巢穴";
	elseif task:GetTaskSubmitNpc(61403) == npcId then
		if Task_Submit_00061403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "逆风巢穴";
		elseif task:HasAcceptedTask(61403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "逆风巢穴";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士不知道你现在是否了解孤冷山的形势，那边的远征军刚送来消息说，许多原始毒龙正在孤冷山的逆风巢穴肆虐，那些毒龙非常厉害，他们想要申请支援，可是我一时没有更多的兵力，能不能麻烦勇士你去帮他们一下。";
	action.m_ActionMsg = "没问题，我最喜欢接受这样的挑战了。";
	return action;
end

function Task_00061403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是厉害，这么快就解决掉那些毒龙了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061403_step_table = {
		[1] = Task_00061403_step_01,
		[10] = Task_00061403_step_10,
		};

function Task_00061403_step(step)
	if Task_00061403_step_table[step] ~= nil then
		return Task_00061403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61403) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61403) then
		return false;
	end


	player:AddExp(80000);
	player:getCoin(75000);
	return true;
end

--��������
function Task_00061403_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61403);
end
