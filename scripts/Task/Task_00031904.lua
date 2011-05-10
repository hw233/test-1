--����Ľ�������
function Task_Accept_00031904()
	local player = GetPlayer();
	if player:GetLev() < 72 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31904) or task:HasCompletedTask(31904) or task:HasSubmitedTask(31904) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31903) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31903) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031904()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 72 then
		return false;
	end
	if task:HasAcceptedTask(31904) or task:HasCompletedTask(31904) or task:HasSubmitedTask(31904) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31903) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31903) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031904()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31904) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031904(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31904) == npcId and Task_Accept_00031904 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31904
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "吕布之魂";
	elseif task:GetTaskSubmitNpc(31904) == npcId then
		if Task_Submit_00031904() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31904
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "吕布之魂";
		elseif task:HasAcceptedTask(31904) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31904
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "吕布之魂";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031904_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你为帮助我复活吕布还是很尽力的，我很感激。现在已经有了还魂丹了，还需要将吕布的魂魄请过来。";
	action.m_ActionMsg = "这要怎么做呢？";
	return action;
end

function Task_00031904_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "他被埋葬在常泉坡的吕布之墓里，他的魂一定也还是留在那里，你只要去跟他对话，让他跟你走，就可以将他的魂魄带回来了。";
	action.m_ActionMsg = "我会尽快把他的魂带来，好早日洗刷我的冤屈。";
	return action;
end

function Task_00031904_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "还好我的爱人的灵魂还在，我们还有机会可以重逢。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031904_step_table = {
		[1] = Task_00031904_step_01,
		[2] = Task_00031904_step_02,
		[10] = Task_00031904_step_10,
		};

function Task_00031904_step(step)
	if Task_00031904_step_table[step] ~= nil then
		return Task_00031904_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031904_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031904() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31904) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15082,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15082, itemNum, 1);
		end
		task:AddTaskStep2(31904, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031904_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15082,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(5033,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31904) then
		return false;
	end

	if IsEquipTypeId(5033) then
		for k = 1, 1 do
			package:AddEquip(5033, 1);
		end
	else 
		package:AddItem(5033,1,1);
	end
	package:DelItemAll(15082,1);

	player:AddExp(260000);
	player:getCoin(245000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031904_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15082,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31904);
end
