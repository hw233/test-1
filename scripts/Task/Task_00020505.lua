--����Ľ�������
function Task_Accept_00020505()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 26 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20505) or task:HasCompletedTask(20505) or task:HasSubmitedTask(20505) then
		return false;
	end
	if not task:HasSubmitedTask(20504) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020505()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 26 then
		return false;
	end
	if task:HasAcceptedTask(20505) or task:HasCompletedTask(20505) or task:HasSubmitedTask(20505) then
		return false;
	end
	if not task:HasSubmitedTask(20504) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020505()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20505) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020505(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20505) == npcId and Task_Accept_00020505 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20505
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "捉拿夜摩";
	elseif task:GetTaskSubmitNpc(20505) == npcId then
		if Task_Submit_00020505() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20505
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "捉拿夜摩";
		elseif task:HasAcceptedTask(20505) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20505
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "捉拿夜摩";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020505_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你尚在成长期就有这么大的勇气和志愿，真是我们魔族的骄傲啊，这样的勇士我是一定乐于帮助的。";
	action.m_ActionMsg = "太好了。";
	return action;
end

function Task_00020505_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在我已经为你施加了强大的法术，你必须在5分钟之内打败那些盘踞在落寞山的毒皮狼人,否则失去我法力的保护你可能会遇到危险。为了荣誉，小心应战，并把战况汇报给摧伏大臣吧。\n";
	action.m_ActionMsg = "好，请相信我。";
	return action;
end

function Task_00020505_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来那里只有狼鬼族？看来夜叉王和摩可拿在这之前已经获得消息逃跑了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020505_step_table = {
		[1] = Task_00020505_step_01,
		[2] = Task_00020505_step_02,
		[10] = Task_00020505_step_10,
		};

function Task_00020505_step(step)
	if Task_00020505_step_table[step] ~= nil then
		return Task_00020505_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020505_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020505() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20505) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00020505_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 126 and itemNum == 1 then
		select = true;
	elseif itemId == 127 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20505) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end

	player:AddExp(3200);
	player:getCoin(5300);
	player:getTael(20);
	return true;
end

--��������
function Task_00020505_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20505);
end
