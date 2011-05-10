--����Ľ�������
function Task_Accept_00020501()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 26 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20501) or task:HasCompletedTask(20501) or task:HasSubmitedTask(20501) then
		return false;
	end
	if not task:HasSubmitedTask(20408) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 26 then
		return false;
	end
	if task:HasAcceptedTask(20501) or task:HasCompletedTask(20501) or task:HasSubmitedTask(20501) then
		return false;
	end
	if not task:HasSubmitedTask(20408) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20501) == npcId and Task_Accept_00020501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "黑暗追踪";
	elseif task:GetTaskSubmitNpc(20501) == npcId then
		if Task_Submit_00020501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "黑暗追踪";
		elseif task:HasAcceptedTask(20501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20501
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "黑暗追踪";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你从什刹海一路上到这里来，一定已经经历了很多事情了，应该已经意识到危险在逼近，不仅是与天族的仇恨在加剧，鬼蜮山这周围，最近已经不太平了，这里是通往山海大陆的出口。";
	action.m_ActionMsg = "原来可以从这里去山海大陆。";
	return action;
end

function Task_00020501_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我怀疑已经有山海大陆的邪恶势力偷偷向我们这里侵袭，最近我的手下不断向我报告发现他们的踪迹，现在我要你去落寞山再确认一下，究竟是怎样的情况。";
	action.m_ActionMsg = "我去落寞山看看。";
	return action;
end

function Task_00020501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "果然是蠢蠢欲动的夜摩盟，他们已经渗透到这里来了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020501_step_table = {
		[1] = Task_00020501_step_01,
		[2] = Task_00020501_step_02,
		[10] = Task_00020501_step_10,
		};

function Task_00020501_step(step)
	if Task_00020501_step_table[step] ~= nil then
		return Task_00020501_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20501) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00020501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20501) then
		return false;
	end


	player:AddExp(3200);
	player:getCoin(4500);
	player:getTael(10);
	return true;
end

--��������
function Task_00020501_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20501);
end
