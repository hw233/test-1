--����Ľ�������
function Task_Accept_00000153()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(153) or task:HasCompletedTask(153) or task:HasSubmitedTask(153) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(152) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(152) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(152) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000153()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(153) or task:HasCompletedTask(153) or task:HasSubmitedTask(153) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(152) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(152) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(152) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000153()
	if GetPlayer():GetTaskMgr():HasCompletedTask(153) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000153(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(153) == npcId and Task_Accept_00000153 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 153
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "仙人虞";
	elseif task:GetTaskSubmitNpc(153) == npcId then
		if Task_Submit_00000153() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 153
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "仙人虞";
		elseif task:HasAcceptedTask(153) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 153
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "仙人虞";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000153_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "雁荡山一带风景秀丽，不过深山之中多毒蛇猛兽出没，据老一辈的传下来说，雁荡山有一株奇异的毒草叫仙人虞，此草可化形千万但是剧毒无比，寻常人稍微吸入一丝立刻七窍流血而亡。这段时间进山采药的村民们不少命丧于此毒草手下，还希望少侠帮助移除这株毒草。";
	action.m_ActionMsg = "还有这种事情？这个毒草在哪里，什么样子？";
	return action;
end

function Task_00000153_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，听我的祖父说起，这毒草一般化为蛇形在山中出没，此蛇人面蛇身应该很好辨认。";
	action.m_ActionMsg = "好，我这就去雁荡山中找出这个仙人虞。";
	return action;
end

function Task_00000153_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是谢天谢地，有少侠你来帮助我们。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000153_step_table = {
		[1] = Task_00000153_step_01,
		[2] = Task_00000153_step_02,
		[10] = Task_00000153_step_10,
		};

function Task_00000153_step(step)
	if Task_00000153_step_table[step] ~= nil then
		return Task_00000153_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000153_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000153() then
		return false;
	end
	if not task:AcceptTask(153) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000153_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(153) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--��������
function Task_00000153_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(153);
end
