--����Ľ�������
function Task_Accept_00061802()
	local player = GetPlayer();
	if player:GetLev() < 66 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61802) or task:HasCompletedTask(61802) or task:HasSubmitedTask(61802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61801) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061802()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 66 then
		return false;
	end
	if task:HasAcceptedTask(61802) or task:HasCompletedTask(61802) or task:HasSubmitedTask(61802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61801) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061802()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61802) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061802(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61802) == npcId and Task_Accept_00061802 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61802
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "手刃猎手";
	elseif task:GetTaskSubmitNpc(61802) == npcId then
		if Task_Submit_00061802() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61802
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "手刃猎手";
		elseif task:HasAcceptedTask(61802) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61802
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "手刃猎手";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061802_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你来的真及时，虽然我不认识你，不过我觉得你不像是来害我的，毕竟你跟刚才偷袭我的那群猎手长的很不一样。";
	action.m_ActionMsg = "你刚刚被偷袭了？";
	return action;
end

function Task_00061802_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是啊，虽然我曾经是所向无敌的，可是来到这山海大陆我似乎变的有点不一样了，我被那些丑陋的浸毒猎手偷袭的时候居然毫无还手之力，简直被害惨了，等我恢复了，我一定绕不了他们。";
	action.m_ActionMsg = "英雄放心，我现在就去替你解决了他们。";
	return action;
end

function Task_00061802_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻人你可真是厉害。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061802_step_table = {
		[1] = Task_00061802_step_01,
		[2] = Task_00061802_step_02,
		[10] = Task_00061802_step_10,
		};

function Task_00061802_step(step)
	if Task_00061802_step_table[step] ~= nil then
		return Task_00061802_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061802_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061802() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61802) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061802_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61802) then
		return false;
	end


	player:AddExp(190000);
	player:getCoin(165000);
	return true;
end

--��������
function Task_00061802_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61802);
end
