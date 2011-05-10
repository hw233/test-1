--����Ľ�������
function Task_Accept_00050904()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50904) or task:HasCompletedTask(50904) or task:HasSubmitedTask(50904) then
		return false;
	end
	if not task:HasSubmitedTask(50903) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050904()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(50904) or task:HasCompletedTask(50904) or task:HasSubmitedTask(50904) then
		return false;
	end
	if not task:HasSubmitedTask(50903) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050904()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50904) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050904(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50904) == npcId and Task_Accept_00050904 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50904
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "磨难的收获";
	elseif task:GetTaskSubmitNpc(50904) == npcId then
		if Task_Submit_00050904() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50904
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "磨难的收获";
		elseif task:HasAcceptedTask(50904) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50904
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "磨难的收获";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050904_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "一直在鬼域山作乱的狼鬼族，现在越来越无法无天了，越来越多的狼鬼族涌入地界，我们必须采取行动，阻止这种势头。落寞山和金阁废墟都被毒皮狼人和变异狼人占据了，这种妖怪不是普通的力量可以对付的，鬼域山附近的族民都深受其苦，现在是你进一步证明自己的时候了。";
	action.m_ActionMsg = "为了要证明自己需要做这么多啊。";
	return action;
end

function Task_00050904_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是的，世事皆无轻而易举，越是磨难收获越是巨大，等你完成了，我会给你非常好的奖励。";
	action.m_ActionMsg = "我知道了，我现在去。";
	return action;
end

function Task_00050904_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "青山依旧，夕阳正红，真希望鬼域山能一直保持这样的平静。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050904_step_table = {
		[1] = Task_00050904_step_01,
		[2] = Task_00050904_step_02,
		[10] = Task_00050904_step_10,
		};

function Task_00050904_step(step)
	if Task_00050904_step_table[step] ~= nil then
		return Task_00050904_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050904_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050904() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50904) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00050904_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8934,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50904) then
		return false;
	end

	if IsEquipTypeId(8934) then
		for k = 1, 1 do
			package:AddEquip(8934, 1);
		end
	else 
		package:AddItem(8934,1,1);
	end

	player:AddExp(4000);
	player:getCoin(5100);
	return true;
end

--��������
function Task_00050904_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50904);
end
