--����Ľ�������
function Task_Accept_00062202()
	local player = GetPlayer();
	if player:GetLev() < 73 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62202) or task:HasCompletedTask(62202) or task:HasSubmitedTask(62202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62201) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 73 then
		return false;
	end
	if task:HasAcceptedTask(62202) or task:HasCompletedTask(62202) or task:HasSubmitedTask(62202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62201) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62202) == npcId and Task_Accept_00062202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "真正的身世";
	elseif task:GetTaskSubmitNpc(62202) == npcId then
		if Task_Submit_00062202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "真正的身世";
		elseif task:HasAcceptedTask(62202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "真正的身世";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我并不认识你，你来找我是想听我唱曲吗？";
	action.m_ActionMsg = "不是的姑娘，我是受人所托，想了解你的真实身世。";
	return action;
end

function Task_00062202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "关于我的真正身世，我真的是毫无了解，我从有记忆以来就住在这绿波宫里，三僧照顾我长大，教会我琴棋歌舞，我也以此为生，却从未向他打听我的真实身世。如果你一定想要知道，就去展旗寺找三僧问问吧。";
	action.m_ActionMsg = "原来是这样，那我去展旗寺了。";
	return action;
end

function Task_00062202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "施主留步。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062202_step_table = {
		[1] = Task_00062202_step_01,
		[2] = Task_00062202_step_02,
		[10] = Task_00062202_step_10,
		};

function Task_00062202_step(step)
	if Task_00062202_step_table[step] ~= nil then
		return Task_00062202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62202) then
		return false;
	end
	task:AddTaskStep(62202);
	return true;
end



--�ύ����
function Task_00062202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62202) then
		return false;
	end


	player:AddExp(280000);
	player:getCoin(259000);
	return true;
end

--��������
function Task_00062202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62202);
end
