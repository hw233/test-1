--����Ľ�������
function Task_Accept_00030303()
	local player = GetPlayer();
	if player:GetLev() < 34 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30303) or task:HasCompletedTask(30303) or task:HasSubmitedTask(30303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30302) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 34 then
		return false;
	end
	if task:HasAcceptedTask(30303) or task:HasCompletedTask(30303) or task:HasSubmitedTask(30303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30302) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30303) == npcId and Task_Accept_00030303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "护送守墓人";
	elseif task:GetTaskAcceptNpc(30303) == npcId and task:HasAcceptedTask(30303) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30303
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "护送守墓人";
	elseif task:GetTaskSubmitNpc(30303) == npcId then
		if Task_Submit_00030303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "护送守墓人";
		elseif task:HasAcceptedTask(30303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "护送守墓人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "刚刚的战斗打的很漂亮，你这位年轻的勇士实力真是不可小觑，慢慢修炼成长吧，山海大陆上的旅程可不是那么容易的，信念和实力一样重要。";
	action.m_ActionMsg = "请继续……";
	return action;
end

function Task_00030303_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我现在要出去一下，去清扫一下阿兰若山谷里的战争碑文，最近经常有新来的勇士到这里来看那块战争的纪念碑，不常清理恐怕落灰了，你呢就顺便护送我一段吧，路上会有些夜摩的怪物很是烦人，老朽懒得跟他们动手了，就由你来代劳吧，完成之后去原来的地方找我，我自己会回去的。";
	action.m_ActionMsg = "这个没问题，有我在，你就把心放肚子里.";
	return action;
end

function Task_00030303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "等你半天了，年轻人。";
	action.m_ActionMsg = "";
	return action;
end

function Task_00030303_step_11()
	RunConveyAction(GetPlayer(),30303);
end

local Task_00030303_step_table = {
		[1] = Task_00030303_step_01,
		[2] = Task_00030303_step_02,
		[10] = Task_00030303_step_10,
		[11] = Task_00030303_step_11,
		};

function Task_00030303_step(step)
	if Task_00030303_step_table[step] ~= nil then
		return Task_00030303_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030303() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30303) then
		return false;
	end
	RunConveyAction(player,30303);
	return true;
end



--�ύ����
function Task_00030303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8914,1,1) + package:GetItemUsedGrids(8918,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30303) then
		return false;
	end

	if IsEquipTypeId(8914) then
		for k = 1, 1 do
			package:AddEquip(8914, 1);
		end
	else 
		package:AddItem(8914,1,1);
	end
	if IsEquipTypeId(8918) then
		for k = 1, 1 do
			package:AddEquip(8918, 1);
		end
	else 
		package:AddItem(8918,1,1);
	end

	player:AddExp(5000);
	player:getCoin(6800);
	player:getTael(20);
	return true;
end

--��������
function Task_00030303_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30303);
end
