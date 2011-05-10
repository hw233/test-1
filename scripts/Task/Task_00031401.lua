--����Ľ�������
function Task_Accept_00031401()
	local player = GetPlayer();
	if player:GetLev() < 62 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31401) or task:HasCompletedTask(31401) or task:HasSubmitedTask(31401) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31304) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31304) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 62 then
		return false;
	end
	if task:HasAcceptedTask(31401) or task:HasCompletedTask(31401) or task:HasSubmitedTask(31401) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31304) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31304) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31401) == npcId and Task_Accept_00031401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "风起云涌";
	elseif task:GetTaskSubmitNpc(31401) == npcId then
		if Task_Submit_00031401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "风起云涌";
		elseif task:HasAcceptedTask(31401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31401
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "风起云涌";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士欢迎你来到猎户山，你的事迹已经在山海大陆传播开了，大家都知道你是实力强大，正义凛然的勇士，尽管有天魔之战的阴影和再次爆发的压力，你仍然坚持自己的信念在山海大陆逐渐闯出自己的世界，实在让我们敬佩。";
	action.m_ActionMsg = "过奖了。";
	return action;
end

function Task_00031401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "实不相瞒，我们镇邪派也面临危急时刻，急需你出手相助，如果你愿意的话，希望你能去鸾羽崖帮我们取一根巫枝回来，夜摩的势力对我们的危险越来越大，我们驻扎在这里的人手实在不够，正在研究以少敌多的办法。";
	action.m_ActionMsg = "不是什么难事，我这就帮你去。";
	return action;
end

function Task_00031401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "太好了，我已经想好了下一步该怎么做了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031401_step_table = {
		[1] = Task_00031401_step_01,
		[2] = Task_00031401_step_02,
		[10] = Task_00031401_step_10,
		};

function Task_00031401_step(step)
	if Task_00031401_step_table[step] ~= nil then
		return Task_00031401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031401() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31401) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15071,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15071, itemNum, 1);
		end
		task:AddTaskStep2(31401, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15071,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(31401) then
		return false;
	end

	package:DelItemAll(15071,1);

	player:AddExp(150000);
	player:getCoin(125000);
	player:getTael(20);
	return true;
end

--��������
function Task_00031401_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15071,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31401);
end
