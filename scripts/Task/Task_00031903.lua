--����Ľ�������
function Task_Accept_00031903()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31903) or task:HasCompletedTask(31903) or task:HasSubmitedTask(31903) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31902) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31902) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031903()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(31903) or task:HasCompletedTask(31903) or task:HasSubmitedTask(31903) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31902) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31902) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031903()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31903) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031903(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31903) == npcId and Task_Accept_00031903 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31903
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "还魂丹";
	elseif task:GetTaskSubmitNpc(31903) == npcId then
		if Task_Submit_00031903() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31903
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "还魂丹";
		elseif task:HasAcceptedTask(31903) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31903
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "还魂丹";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031903_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士不管吕布的死跟你是不是有直接关系，我都很高兴你愿意帮我。";
	action.m_ActionMsg = "举手之劳，但希望你能相信我，吕布并不是我杀的。";
	return action;
end

function Task_00031903_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我需要血浸废墟的还魂鬼族所持有的那种特别的还魂丹，只有杀掉他们才能从他们的尸体获得。";
	action.m_ActionMsg = "没问题，但请你一定要相信我，我从不乱杀无辜。";
	return action;
end

function Task_00031903_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "获得还魂丹是我们的复活吕布很重要的一步，你这就帮我完成了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031903_step_table = {
		[1] = Task_00031903_step_01,
		[2] = Task_00031903_step_02,
		[10] = Task_00031903_step_10,
		};

function Task_00031903_step(step)
	if Task_00031903_step_table[step] ~= nil then
		return Task_00031903_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031903_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031903() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31903) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15076,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15076, itemNum, 1);
		end
		task:AddTaskStep2(31903, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031903_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15076,1) < 12 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(843,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31903) then
		return false;
	end

	if IsEquipTypeId(843) then
		for k = 1, 1 do
			package:AddEquip(843, 1);
		end
	else 
		package:AddItem(843,1,1);
	end
	package:DelItemAll(15076,1);

	player:AddExp(230000);
	player:getCoin(217500);
	player:getTael(40);
	return true;
end

--��������
function Task_00031903_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15076,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31903);
end
