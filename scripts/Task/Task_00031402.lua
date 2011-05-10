--����Ľ�������
function Task_Accept_00031402()
	local player = GetPlayer();
	if player:GetLev() < 62 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31402) or task:HasCompletedTask(31402) or task:HasSubmitedTask(31402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31401) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031402()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 62 then
		return false;
	end
	if task:HasAcceptedTask(31402) or task:HasCompletedTask(31402) or task:HasSubmitedTask(31402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31401) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031402()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31402) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031402(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31402) == npcId and Task_Accept_00031402 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31402
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "夜黑风高";
	elseif task:GetTaskSubmitNpc(31402) == npcId then
		if Task_Submit_00031402() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31402
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "夜黑风高";
		elseif task:HasAcceptedTask(31402) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31402
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "夜黑风高";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031402_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你帮我们取回来的那根巫枝，不是普通的树枝，而是一根沉浸了巫术的树枝，我已经将其中的巫术提取出来汇成了毒气，并封存在这个瓦罐里。";
	action.m_ActionMsg = "是要我去放毒？";
	return action;
end

function Task_00031402_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是的，猾口林的狼族守卫，数量众多，极难对付，但是他们又把守着唯一能打开封禁着罗阅王的塔的钥匙。于是我想出了用毒气使他们昏睡的办法，如果你愿意帮助我们到底，请在猾口林将这个瓦罐打破，使毒气弥漫，很快他们就会昏睡过去了。";
	action.m_ActionMsg = "没问题，我这就去。";
	return action;
end

function Task_00031402_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我相信我这个办法一定很有效。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031402_step_table = {
		[1] = Task_00031402_step_01,
		[2] = Task_00031402_step_02,
		[10] = Task_00031402_step_10,
		};

function Task_00031402_step(step)
	if Task_00031402_step_table[step] ~= nil then
		return Task_00031402_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031402_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031402() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15072, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(31402) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031402_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15072,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8915,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31402) then
		return false;
	end

	if IsEquipTypeId(8915) then
		for k = 1, 1 do
			package:AddEquip(8915, 1);
		end
	else 
		package:AddItem(8915,1,1);
	end
	package:DelItemAll(15072,1);

	player:AddExp(150000);
	player:getCoin(125000);
	player:getTael(20);
	return true;
end

--��������
function Task_00031402_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15072,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31402);
end
