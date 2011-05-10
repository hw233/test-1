--����Ľ�������
function Task_Accept_00010404()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 23 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10404) or task:HasCompletedTask(10404) or task:HasSubmitedTask(10404) then
		return false;
	end
	if not task:HasSubmitedTask(10403) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010404()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 23 then
		return false;
	end
	if task:HasAcceptedTask(10404) or task:HasCompletedTask(10404) or task:HasSubmitedTask(10404) then
		return false;
	end
	if not task:HasSubmitedTask(10403) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010404()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10404) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010404(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10404) == npcId and Task_Accept_00010404 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10404
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "战争的遗迹";
	elseif task:GetTaskSubmitNpc(10404) == npcId then
		if Task_Submit_00010404() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10404
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "战争的遗迹";
		elseif task:HasAcceptedTask(10404) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10404
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "战争的遗迹";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010404_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "战争是残酷的，但是维护天族的利益也是我们的责任，只是我们兄弟再不能相见了。";
	action.m_ActionMsg = "（叹气）别难过了，已经成为历史了。";
	return action;
end

function Task_00010404_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在你去位于山海大陆的阿兰若山谷看看战争的遗迹吧，你就会知后把他带给我。";
	action.m_ActionMsg = "我倒是很有兴趣去看看当时的战场。";
	return action;
end

function Task_00010404_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这块小石碑，承载了很多对于那场战争的记忆啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010404_step_table = {
		[1] = Task_00010404_step_01,
		[2] = Task_00010404_step_02,
		[10] = Task_00010404_step_10,
		};

function Task_00010404_step(step)
	if Task_00010404_step_table[step] ~= nil then
		return Task_00010404_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010404_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010404() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10404) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15026,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15026, itemNum, 1);
		end
		task:AddTaskStep2(10404, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00010404_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 130 and itemNum == 1 then
		select = true;
	elseif itemId == 132 and itemNum == 1 then
		select = true;
	elseif itemId == 134 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();
	if package:GetItemNum(15026,1) < 1 then
		return false;
	end

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10404) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end
	package:DelItemAll(15026,1);

	player:AddExp(2200);
	player:getCoin(4100);
	player:getTael(10);
	return true;
end

--��������
function Task_00010404_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15026,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10404);
end
