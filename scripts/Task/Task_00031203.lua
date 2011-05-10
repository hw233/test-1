--����Ľ�������
function Task_Accept_00031203()
	local player = GetPlayer();
	if player:GetLev() < 59 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31203) or task:HasCompletedTask(31203) or task:HasSubmitedTask(31203) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31202) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31202) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 59 then
		return false;
	end
	if task:HasAcceptedTask(31203) or task:HasCompletedTask(31203) or task:HasSubmitedTask(31203) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31202) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31202) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31203) == npcId and Task_Accept_00031203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "挑战摩睺";
	elseif task:GetTaskSubmitNpc(31203) == npcId then
		if Task_Submit_00031203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "挑战摩睺";
		elseif task:HasAcceptedTask(31203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "挑战摩睺";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "哈哈哈，你这个呆小子，什么也不懂，还妄图从我这里获得你想要的名将，真是痴心妄想。";
	action.m_ActionMsg = "你这个无耻的家伙，我早该猜到你是夜摩盟的了。";
	return action;
end

function Task_00031203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "只能说你太笨了，你这样的人就不该继续活着，我会让你吃尽苦头的。";
	action.m_ActionMsg = "你的阴谋迟早会暴露，我会尽我所能阻止你的。";
	return action;
end

function Task_00031203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士遇到什么情况了吗，告诉我看我能不能帮到你？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031203_step_table = {
		[1] = Task_00031203_step_01,
		[2] = Task_00031203_step_02,
		[10] = Task_00031203_step_10,
		};

function Task_00031203_step(step)
	if Task_00031203_step_table[step] ~= nil then
		return Task_00031203_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031203() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31203) then
		return false;
	end
	RunAutoBattleAction(player,4260,100);
	task:AddTaskStep(31203);

	return true;
end



--�ύ����
function Task_00031203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(703,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31203) then
		return false;
	end

	if IsEquipTypeId(703) then
		for k = 1, 1 do
			package:AddEquip(703, 1);
		end
	else 
		package:AddItem(703,1,1);
	end

	player:AddExp(100000);
	player:getCoin(100000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031203_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31203);
end
