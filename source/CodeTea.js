const triSpin = "tangbao_triSpin"
const meaus = new Map();
meaus.set("meau1", "_home")
meaus.set("meau2", "_notes")
meaus.set("meau3", "_search")
meaus.set("meau4", "_teaLife")

function meauChange(_id)
{   
    _idChildre = document.getElementById(_id).parentNode.children
    
    for(var i=0; i<_idChildre.length; i++)
        _idChildre[i].classList.remove(triSpin)
    document.getElementById(_id).classList.add(triSpin)

    
    for(var value of meaus.values()) // JS 无法判断元素是否存在，所以必须对应
    {
        // console.log(String(value))
        document.getElementById(String(value)).style.display = "none"
    }
        
    
    document.getElementById(meaus.get(_id)).style.display = "unset"
}