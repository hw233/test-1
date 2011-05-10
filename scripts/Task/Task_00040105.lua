--����Ľ�������
function Task_Accept_00040105()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40105) or task:HasCompletedTask(40105) or task:HasSubmitedTask(40105) then
		return false;
	end
	if not task:HasSubmitedTask(40104) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(40105) or task:HasCompletedTask(40105) or task:HasSubmitedTask(40105) then
		return false;
	end
	if not task:HasSubmitedTask(40104) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40105) == npcId and Task_Accept_00040105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "伤感与沉默";
	elseif task:GetTaskSubmitNpc(40105) == npcId then
		if Task_Submit_00040105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "伤感与沉默";
		elseif task:HasAcceptedTask(40105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "伤感与沉默";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "（脸上掠过一丝伤感）勇士，谢谢你为我带来这件衣服，我想不用你说我已经知道结果了。";
	action.m_ActionMsg = "可是……王还是让我带话给你啊。";
	return action;
end

function Task_00040105_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "谢谢你的好意，亲爱的勇士，我想王的意思已经很明显了，他不会再对我有兴趣了，我也只能接受他的安慰，你也会为你的辛劳得到奖励。";
	action.m_ActionMsg = "还好我没有白跑一趟。";
	return action;
end

function Task_00040105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "无言独上西楼， \n月如钩， \n寂寞梧桐深院锁清秋。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040105_step_table = {
		[1] = Task_00040105_step_01,
		[2] = Task_00040105_step_02,
		[10] = Task_00040105_step_10,
		};

function Task_00040105_step(step)
	if Task_00040105_step_table[step] ~= nil then
		return Task_00040105_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40105) then
		return false;
	end
	task:AddTaskStep(40105);
	return true;
end



--�ύ����
function Task_00040105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8900,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40105) then
		return false;
	end

	if IsEquipTypeId(8900) then
		for k = 1, 1 do
			package:AddEquip(8900, 1);
		end
	else 
		package:AddItem(8900,1,1);
	end

	player:AddExp(800);
	player:getCoin(700);
	return true;
end

--��������
function Task_00040105_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40105);
end
