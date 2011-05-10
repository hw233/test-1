--����Ľ�������
function Task_Accept_00062103()
	local player = GetPlayer();
	if player:GetLev() < 73 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62103) or task:HasCompletedTask(62103) or task:HasSubmitedTask(62103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62102) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 73 then
		return false;
	end
	if task:HasAcceptedTask(62103) or task:HasCompletedTask(62103) or task:HasSubmitedTask(62103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62102) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62103) == npcId and Task_Accept_00062103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "解决侵扰";
	elseif task:GetTaskSubmitNpc(62103) == npcId then
		if Task_Submit_00062103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "解决侵扰";
		elseif task:HasAcceptedTask(62103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "解决侵扰";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我看你实力非凡，似乎山海大陆上就没什么是你不能战胜的，我在的这个地方，虽然有很多值得挖掘的古迹，可是却人迹罕至，我经常受到一些邪恶势力的骚扰，自己却无能为力。";
	action.m_ActionMsg = "那怪物是不是夜摩盟的爪牙？我也一直在猎杀夜摩盟的势力。";
	return action;
end

function Task_00062103_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那太好了，灭境岩的裂化渔人和凌波阵的邪毒水妖，我没有跟他们正面对峙过，但也许他们就是夜摩盟的势力，勇士你可以去帮我解决掉他们吗，我想在这里好好的挖墓，不想再受到侵扰了。";
	action.m_ActionMsg = "没问题，就交给我了。";
	return action;
end

function Task_00062103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "太好了，接下来我又可以不受侵扰的做自己的事情了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062103_step_table = {
		[1] = Task_00062103_step_01,
		[2] = Task_00062103_step_02,
		[10] = Task_00062103_step_10,
		};

function Task_00062103_step(step)
	if Task_00062103_step_table[step] ~= nil then
		return Task_00062103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62103) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00062103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62103) then
		return false;
	end


	player:AddExp(280000);
	player:getCoin(259000);
	return true;
end

--��������
function Task_00062103_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62103);
end
