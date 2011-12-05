--����Ľ�������
function Task_Accept_00000089()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(89) or task:HasCompletedTask(89) or task:HasSubmitedTask(89) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000089()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(89) or task:HasCompletedTask(89) or task:HasSubmitedTask(89) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000089()
	if GetPlayer():GetTaskMgr():HasCompletedTask(89) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000089(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(89) == npcId and Task_Accept_00000089 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 89
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "追杀野魔";
	elseif task:GetTaskSubmitNpc(89) == npcId then
		if Task_Submit_00000089() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 89
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "追杀野魔";
		elseif task:HasAcceptedTask(89) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 89
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "追杀野魔";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000089_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这个雅各达绰号西方野魔，颇有几件厉害的法宝，尤其是他的黄沙魔火歹毒无比，因为地窍中的雪魂珠是他魔火的克星，所以他也觊觎雪魂珠许久，常常来玄冰谷骚扰我。";
	action.m_ActionMsg = "这个雅各达确实非常可恶。";
	return action;
end

function Task_00000089_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "他一般在鬼风谷炼制魔火，这次被你击败一定是跑回了鬼风谷，所谓除恶务尽，这个雅各达一定不要放过，否则后患无穷。"..GetPlayerName(GetPlayer()).."你应该立刻去鬼风谷将他消灭。";
	action.m_ActionMsg = "前辈说的有理，我这就赶去鬼风谷。";
	return action;
end

function Task_00000089_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢"..GetPlayerName(GetPlayer()).."你的帮助，日后我枯禅参悟完毕，一定将雪魂珠送你。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000089_step_table = {
		[1] = Task_00000089_step_01,
		[2] = Task_00000089_step_02,
		[10] = Task_00000089_step_10,
		};

function Task_00000089_step(step)
	if Task_00000089_step_table[step] ~= nil then
		return Task_00000089_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000089_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000089() then
		return false;
	end
	if not task:AcceptTask(89) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000089_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(89) then
		return false;
	end


	player:AddExp(25000);
	return true;
end

--��������
function Task_00000089_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(89);
end
