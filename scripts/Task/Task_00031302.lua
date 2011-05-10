--����Ľ�������
function Task_Accept_00031302()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31302) or task:HasCompletedTask(31302) or task:HasSubmitedTask(31302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31301) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(31302) or task:HasCompletedTask(31302) or task:HasSubmitedTask(31302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31301) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31302) == npcId and Task_Accept_00031302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "抢回食物";
	elseif task:GetTaskSubmitNpc(31302) == npcId then
		if Task_Submit_00031302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "抢回食物";
		elseif task:HasAcceptedTask(31302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "抢回食物";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "非常感谢你，勇士"..GetPlayerName(GetPlayer()).."，镇邪派为有你这样的朋友感到荣幸，如果不是你，我肯定回不到这里了，也没有办法这么快恢复身体状况。";
	action.m_ActionMsg = "你太客气了，夜摩盟也是我的敌人。";
	return action;
end

function Task_00031302_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是的，夜摩盟的贪欲已经到了肆无忌惮的地步，那些占据了灵山塔的残肢狼人，受了夜摩的指使，将我们这些驻外部队的食物储备抢走了，我非常着急，可是虽然身体有所恢复，但是战斗力还没有完全恢复，希望你能帮我们这个忙，打败那些狼人，将我们的食物储备夺回来。";
	action.m_ActionMsg = "没问题，我现在就去帮你。";
	return action;
end

function Task_00031302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你帮了我这么多，真是不知道该怎么感谢你了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031302_step_table = {
		[1] = Task_00031302_step_01,
		[2] = Task_00031302_step_02,
		[10] = Task_00031302_step_10,
		};

function Task_00031302_step(step)
	if Task_00031302_step_table[step] ~= nil then
		return Task_00031302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31302) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15068,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15068, itemNum, 1);
		end
		task:AddTaskStep2(31302, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15068,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8915,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31302) then
		return false;
	end

	if IsEquipTypeId(8915) then
		for k = 1, 1 do
			package:AddEquip(8915, 1);
		end
	else 
		package:AddItem(8915,1,1);
	end
	package:DelItemAll(15068,1);

	player:AddExp(110000);
	player:getCoin(107500);
	player:getTael(40);
	return true;
end

--��������
function Task_00031302_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15068,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31302);
end
