--����Ľ�������
function Task_Accept_00000090()
	local player = GetPlayer();
	if player:GetLev() < 46 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90) or task:HasCompletedTask(90) or task:HasSubmitedTask(90) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000090()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 46 then
		return false;
	end
	if task:HasAcceptedTask(90) or task:HasCompletedTask(90) or task:HasSubmitedTask(90) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000090()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000090(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90) == npcId and Task_Accept_00000090 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "绿袍之难";
	elseif task:GetTaskSubmitNpc(90) == npcId then
		if Task_Submit_00000090() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "绿袍之难";
		elseif task:HasAcceptedTask(90) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "绿袍之难";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000090_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "桀……桀……桀，小辈你不要东张西望了，此处被我那恶徒辛辰子用魔泉幡设下了禁制，就算你道行再高也离不开此峰，原本我可破此法术，只是我的元神被劣徒用九子母元阳针禁锢无法施展魔功，也出不得此峰。";
	action.m_ActionMsg = "那该如何是好。";
	return action;
end

function Task_00000090_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我看你资质不错，仙根福厚，不若拜我为师，我便传你破这九子母元阳针之法，届时我自可施展魔功破了这幡。到时候天下美女财物任你掠夺，天下生灵任你宰割，岂不快哉。";
	action.m_ActionMsg = "（这个绿袍一看就是邪道妖人……不如我假装答应，等脱困之后再做打算）老祖你魔功盖世，能做你的徒弟真是莫大的福分。";
	return action;
end

function Task_00000090_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "徒儿，你还真是乖巧啊……桀桀……桀！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000090_step_table = {
		[1] = Task_00000090_step_01,
		[2] = Task_00000090_step_02,
		[10] = Task_00000090_step_10,
		};

function Task_00000090_step(step)
	if Task_00000090_step_table[step] ~= nil then
		return Task_00000090_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000090_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000090() then
		return false;
	end
	if not task:AcceptTask(90) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000090_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1548,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(90) then
		return false;
	end

	if IsEquipTypeId(1548) then
		for k = 1, 1 do
			package:AddEquip(1548, 1);
		end
	else 
		package:AddItem(1548,1,1);
	end

	player:AddExp(20000);
	return true;
end

--��������
function Task_00000090_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(90);
end
