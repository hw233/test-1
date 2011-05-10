--����Ľ�������
function Task_Accept_00010402()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 23 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10402) or task:HasCompletedTask(10402) or task:HasSubmitedTask(10402) then
		return false;
	end
	if not task:HasSubmitedTask(10401) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010402()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 23 then
		return false;
	end
	if task:HasAcceptedTask(10402) or task:HasCompletedTask(10402) or task:HasSubmitedTask(10402) then
		return false;
	end
	if not task:HasSubmitedTask(10401) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010402()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10402) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010402(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10402) == npcId and Task_Accept_00010402 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10402
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "战争的回忆";
	elseif task:GetTaskSubmitNpc(10402) == npcId then
		if Task_Submit_00010402() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10402
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "战争的回忆";
		elseif task:HasAcceptedTask(10402) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10402
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "战争的回忆";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010402_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "那场天魔之战，是我们天族每个人都不能忘记的充满血泪的毁灭之战，暴躁的阿修罗只是因为帝释天看上了悦意，就杀了我们前去提亲的乐师，并且发起了那场战争。";
	action.m_ActionMsg = "这魔族的阿修罗的做法还真是过分。";
	return action;
end

function Task_00010402_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "就是这场战争，夺去了我们最好的兄弟神秀的生命，他是那么的勇敢而善战。有的时候我会独自去他的坟上，陪他喝他最爱的美酒，给他讲天族新来的漂亮女子。如果你想知道更多，去善见城找当事人悦意王妃吧，我想独自在这里静一静。";
	action.m_ActionMsg = "果然跟悦意王妃有关啊，那我倒要去问个清楚。";
	return action;
end

function Task_00010402_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么说你是来了解天魔之战的情况的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010402_step_table = {
		[1] = Task_00010402_step_01,
		[2] = Task_00010402_step_02,
		[10] = Task_00010402_step_10,
		};

function Task_00010402_step(step)
	if Task_00010402_step_table[step] ~= nil then
		return Task_00010402_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010402_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010402() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10402) then
		return false;
	end
	task:AddTaskStep(10402);
	return true;
end



--�ύ����
function Task_00010402_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10402) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end

	player:AddExp(2200);
	player:getCoin(3700);
	return true;
end

--��������
function Task_00010402_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10402);
end
