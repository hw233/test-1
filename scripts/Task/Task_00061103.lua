--����Ľ�������
function Task_Accept_00061103()
	local player = GetPlayer();
	if player:GetLev() < 48 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61103) or task:HasCompletedTask(61103) or task:HasSubmitedTask(61103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61102) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 48 then
		return false;
	end
	if task:HasAcceptedTask(61103) or task:HasCompletedTask(61103) or task:HasSubmitedTask(61103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61102) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61103) == npcId and Task_Accept_00061103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "太平公主";
	elseif task:GetTaskSubmitNpc(61103) == npcId then
		if Task_Submit_00061103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "太平公主";
		elseif task:HasAcceptedTask(61103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "太平公主";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你在这天池也待了一段时间了，喜欢这里的风景吧，这天然形成的高山湖泊，可是非常独特的风景，几乎人见人爱呢，我甚至听说，有一位叫太平公主的名将因为被这天池的风景迷住，而来到了附近的六祖塔。";
	action.m_ActionMsg = "这点我倒是赞同，这天池的风景的确非常怡人。";
	return action;
end

function Task_00061103_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那太平公主看名字就知道不是个省油的灯，看来要跟他和平相处，我得有点行动，麻烦你代我去拜访一下那位公主吧，顺便带去我最拿手的志怪点心送给她，希望以后能在天池跟她和平相处。";
	action.m_ActionMsg = "好吧，我去找那太平公主。";
	return action;
end

function Task_00061103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，你来见我就好了，怎么还带了东西。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061103_step_table = {
		[1] = Task_00061103_step_01,
		[2] = Task_00061103_step_02,
		[10] = Task_00061103_step_10,
		};

function Task_00061103_step(step)
	if Task_00061103_step_table[step] ~= nil then
		return Task_00061103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061103() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15011, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(61103) then
		return false;
	end
	task:AddTaskStep(61103);
	return true;
end



--�ύ����
function Task_00061103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15011,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61103) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end
	package:DelItemAll(15011,1);

	player:AddExp(48000);
	player:getCoin(34000);
	return true;
end

--��������
function Task_00061103_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15011,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61103);
end
