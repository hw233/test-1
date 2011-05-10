--����Ľ�������
function Task_Accept_00040402()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40402) or task:HasCompletedTask(40402) or task:HasSubmitedTask(40402) then
		return false;
	end
	if not task:HasSubmitedTask(40401) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040402()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(40402) or task:HasCompletedTask(40402) or task:HasSubmitedTask(40402) then
		return false;
	end
	if not task:HasSubmitedTask(40401) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040402()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40402) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040402(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40402) == npcId and Task_Accept_00040402 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40402
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "醉心历练";
	elseif task:GetTaskSubmitNpc(40402) == npcId then
		if Task_Submit_00040402() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40402
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "醉心历练";
		elseif task:HasAcceptedTask(40402) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40402
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "醉心历练";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040402_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "梵天还真是醉心于对我们天族勇士的历练啊，就算是他让你来找我要长生不老药，我也不会很轻易的就让你得到。";
	action.m_ActionMsg = "那你想让我怎么做呢？";
	return action;
end

function Task_00040402_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我首先要知道，你是否已经达到了能经受的住未来苦难的磨练的地步，瞻部峰的奇沙阵那里，盘踞了许多狂蛮猎人，他们的强大战斗力让很多人望而生畏，如果你能够解决掉他们，再回来找我吧。";
	action.m_ActionMsg = "没有什么可以吓住我的。";
	return action;
end

function Task_00040402_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "做的很漂亮，怪不得梵天会推荐你来，他定是看中了你非凡的潜力。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040402_step_table = {
		[1] = Task_00040402_step_01,
		[2] = Task_00040402_step_02,
		[10] = Task_00040402_step_10,
		};

function Task_00040402_step(step)
	if Task_00040402_step_table[step] ~= nil then
		return Task_00040402_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040402_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040402() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40402) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040402_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 42 and itemNum == 1 then
		select = true;
	elseif itemId == 43 and itemNum == 1 then
		select = true;
	elseif itemId == 44 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40402) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end

	player:AddExp(1200);
	player:getCoin(1000);
	return true;
end

--��������
function Task_00040402_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40402);
end
