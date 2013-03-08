-- 给以下ID增加相应等级的GM权限
gm = {}
-- 删除以下ID的GM权限
delgm = {}

function _addGM()
    for k,v in pairs(gm)
    do
        addGM(v[1], v[2])
    end
end

function _delGM()
    for k,v in pairs(delgm)
    do
        delGM(v);
    end
end

_addGM(gm);
_delGM(delgm);

