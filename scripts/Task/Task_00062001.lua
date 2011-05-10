--����Ľ�������
function Task_Accept_00062001()
	local player = GetPlayer();
	if player:GetLev() < 71 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62001) or task:HasCompletedTask(62001) or task:HasSubmitedTask(62001) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062001()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 71 then
		return false;
	end
	if task:HasAcceptedTask(62001) or task:HasCompletedTask(62001) or task:HasSubmitedTask(62001) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00062001()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62001) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062001(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62001) == npcId and Task_Accept_00062001 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62001
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "求神问卦";
	elseif task:GetTaskSubmitNpc(62001) == npcId then
		if Task_Submit_00062001() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62001
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "求神问卦";
		elseif task:HasAcceptedTask(62001) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62001
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "求神问卦";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062001_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我听说无忧谷有一位非常神准的算命大师啊，你听说过吗？";
	action.m_ActionMsg = "我没有听说过什么算命大师";
	return action;
end

function Task_00062001_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "其实你也不妨去找他卜上一卦，顺便也帮我问问，我感觉自己前途未卜，山海大陆情势纷繁复杂，有各占一方的天魔两族，还有征战不休的夜摩东山等等，真是无从下手，你不妨去帮我找无尘大师问一下，我到底夜摩盟会不会在山海大陆获胜，我该向谁效力呢。";
	action.m_ActionMsg = "我自己是不需要了，不过我可以帮你去问一下。";
	return action;
end

function Task_00062001_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "天灵灵，地灵灵，山海大陆我最灵。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062001_step_table = {
		[1] = Task_00062001_step_01,
		[2] = Task_00062001_step_02,
		[10] = Task_00062001_step_10,
		};

function Task_00062001_step(step)
	if Task_00062001_step_table[step] ~= nil then
		return Task_00062001_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062001_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062001() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62001) then
		return false;
	end
	task:AddTaskStep(62001);
	return true;
end



--�ύ����
function Task_00062001_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62001) then
		return false;
	end


	player:AddExp(250000);
	player:getCoin(230000);
	return true;
end

--��������
function Task_00062001_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62001);
end
