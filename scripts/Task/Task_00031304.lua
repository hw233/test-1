--����Ľ�������
function Task_Accept_00031304()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31304) or task:HasCompletedTask(31304) or task:HasSubmitedTask(31304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31303) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(31304) or task:HasCompletedTask(31304) or task:HasSubmitedTask(31304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31303) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31304) == npcId and Task_Accept_00031304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "分发暗器";
	elseif task:GetTaskSubmitNpc(31304) == npcId then
		if Task_Submit_00031304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "分发暗器";
		elseif task:HasAcceptedTask(31304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "分发暗器";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你真的是帮了我们太多忙了，我知道你在山海大陆的旅程并不会停驻在我们这里，所以当你离开摩加湖以后，在经过河西冰道的河西驿站的时候，顺便帮我把最近研制出来的镇邪独门暗器交给驻扎在那里的镇邪后备部队。";
	action.m_ActionMsg = "没问题，我会去那里一趟。";
	return action;
end

function Task_00031304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你也知道我们镇邪派人数势力上都不占有优势，而山海大陆的形势却是越来越严峻，我下了很多功夫研究出来这种可以以少敌多的暗器，希望能帮助到那些独自守卫河西冰道的那些部队。";
	action.m_ActionMsg = "那么请您保重，再会了。";
	return action;
end

function Task_00031304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "太好了，有了这些暗器的武装，相信我们可以更好的驻守边防了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031304_step_table = {
		[1] = Task_00031304_step_01,
		[2] = Task_00031304_step_02,
		[10] = Task_00031304_step_10,
		};

function Task_00031304_step(step)
	if Task_00031304_step_table[step] ~= nil then
		return Task_00031304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031304() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15070, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(31304) then
		return false;
	end
	task:AddTaskStep(31304);
	return true;
end



--�ύ����
function Task_00031304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15070,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(31304) then
		return false;
	end

	package:DelItemAll(15070,1);

	player:AddExp(110000);
	player:getCoin(107500);
	player:getTael(40);
	return true;
end

--��������
function Task_00031304_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15070,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31304);
end
