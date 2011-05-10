--����Ľ�������
function Task_Accept_00031604()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 67 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31604) or task:HasCompletedTask(31604) or task:HasSubmitedTask(31604) then
		return false;
	end
	if not task:HasSubmitedTask(31603) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031604()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 67 then
		return false;
	end
	if task:HasAcceptedTask(31604) or task:HasCompletedTask(31604) or task:HasSubmitedTask(31604) then
		return false;
	end
	if not task:HasSubmitedTask(31603) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031604()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31604) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031604(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31604) == npcId and Task_Accept_00031604 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31604
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "厌倦争斗";
	elseif task:GetTaskSubmitNpc(31604) == npcId then
		if Task_Submit_00031604() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31604
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "厌倦争斗";
		elseif task:HasAcceptedTask(31604) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31604
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "厌倦争斗";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031604_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你这位勇士，果然是实力非同一般，我暂且相信你所说的，可是失去招募吕布的机会的确让我感到惋惜，希望你以后谨慎行事，我不希望再有类似的事情发生，把我们都拖向战争的边缘。";
	action.m_ActionMsg = "别以为是我想这样。";
	return action;
end

function Task_00031604_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然我很厌恶你们天族的狂妄，可是天魔之战的痛苦回忆还经常折磨着我，我知道新的战争随时都可能爆发，可是我不希望是由你引起的，如果真的发生我们都还是要面对。你走吧，去告诉光盖天，这个事情就先算了，以后别再让我看到你了。";
	action.m_ActionMsg = "我也不会再来了。";
	return action;
end

function Task_00031604_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那无聊的魔族的和修罗，理他作甚。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031604_step_table = {
		[1] = Task_00031604_step_01,
		[2] = Task_00031604_step_02,
		[10] = Task_00031604_step_10,
		};

function Task_00031604_step(step)
	if Task_00031604_step_table[step] ~= nil then
		return Task_00031604_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031604_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031604() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31604) then
		return false;
	end
	task:AddTaskStep(31604);
	return true;
end



--�ύ����
function Task_00031604_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8902,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31604) then
		return false;
	end

	if IsEquipTypeId(8902) then
		for k = 1, 1 do
			package:AddEquip(8902, 1);
		end
	else 
		package:AddItem(8902,1,1);
	end

	player:AddExp(200000);
	player:getCoin(180000);
	player:getTael(50);
	return true;
end

--��������
function Task_00031604_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31604);
end
