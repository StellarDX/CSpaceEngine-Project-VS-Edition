#include "../headers/math/CSEM_Func.h"

// Logarithm function implemented by OpenCV
// OpenCV Doc: https://docs.opencv.org/4.x/d3/ddd/group__core__hal__functions.html#ga2b1d931c2cdadf3623d57a853b370147

// Same as exp(x), ln(x) also need to be converted to log2(x)
// According to Logarithms change of base rule, ln(x) can be expressed as log2(x) * ln(2)
// so the integer part of result can be calculated with the formula above.

// Fractional part will be calculated with taylor series
// the expansion of ln(1 + x) is: SUM<0, +inf>((pow(-1, 1 + n) / n) * pow(x, n))
// This formula is also called "Newton–Mercator series"
// https://en.wikipedia.org/wiki/Mercator_series

_CSE_BEGIN

// A table is also used in this function.
// this table has 512 elements, divided into 2 columns.
// the first column stores the value of ln(n / 256 + 1) (n = 0 to 255)
// the second column stores the value of 1 / (n / 256 + 1) (n = 0 to 255)
static const float96 _Logarithm_Tab[]
(
	0.00000000000000000000000000000000L, 1.00000000000000000000000000000000L,
	0.00389864041565732301393734309584L, 0.99610894941634241245136186770428L,
	0.00778214044205494894746290006114L, 0.99224806201550387596899224806202L,
	0.01165061721997527413559144280921L, 0.98841698841698841698841698841699L,
	0.01550418653596525415085404604245L, 0.98461538461538461538461538461538L,
	0.01934296284313093463590553454155L, 0.98084291187739463601532567049808L,
	0.02316705928153437822879916096229L, 0.97709923664122137404580152671756L,
	0.02697658769820207574806929253966L, 0.97338403041825095057034220532319L,
	0.03077165866675368837102820759677L, 0.96969696969696969696969696969697L,
	0.03455238150665973340737150058983L, 0.96603773584905660377358490566038L,
	0.03831886430213659919375532512380L, 0.96240601503759398496240601503759L,
	0.04207121392068705437520380592696L, 0.95880149812734082397003745318352L,
	0.04580953603129420316667926761466L, 0.95522388059701492537313432835821L,
	0.04953393512227663088209620882982L, 0.95167286245353159851301115241636L,
	0.05324451451881228286587019378653L, 0.94814814814814814814814814814815L,
	0.05694137640013842475901310154045L, 0.94464944649446494464944649446494L,
	0.06062462181643484258060613204042L, 0.94117647058823529411764705882353L,
	0.06429435070539725721622845026561L, 0.93772893772893772893772893772894L,
	0.06795066190850774939456527777263L, 0.93430656934306569343065693430657L,
	0.07159365318700881792560527275209L, 0.93090909090909090909090909090909L,
	0.07522342123758752569860533998366L, 0.92753623188405797101449275362319L,
	0.07884006170777602453154057785920L, 0.92418772563176895306859205776173L,
	0.08244366921107459126816006866831L, 0.92086330935251798561151079136691L,
	0.08603433734180315338179782672200L, 0.91756272401433691756272401433692L,
	0.08961215868968713261995146937848L, 0.91428571428571428571428571428571L,
	0.09317722485418328976878135302776L, 0.91103202846975088967971530249110L,
	0.09672962645855111229557105648746L, 0.90780141843971631205673758865248L,
	0.10026945316367514930813017512973L, 0.90459363957597173144876325088339L,
	0.10379679368164356482606180376397L, 0.90140845070422535211267605633803L,
	0.10731173578908805066717503037115L, 0.89824561403508771929824561403509L,
	0.11081436634029011419480616932321L, 0.89510489510489510489510489510490L,
	0.11430477128005863363425914481517L, 0.89198606271777003484320557491289L,
	0.11778303565638345453879410947052L, 0.88888888888888888888888888888889L,
	0.12124924363286968516121226408084L, 0.88581314878892733564013840830450L,
	0.12470347850095723586340651538086L, 0.88275862068965517241379310344828L,
	0.12814582269193003817410988696107L, 0.87972508591065292096219931271478L,
	0.13157635778871927258871612868948L, 0.87671232876712328767123287671233L,
	0.13499516453750483060198329114709L, 0.87372013651877133105802047781570L,
	0.13840232285911913568532587360165L, 0.87074829931972789115646258503401L,
	0.14179791186025734987895273528047L, 0.86779661016949152542372881355932L,
	0.14518200984449789728193506374056L, 0.86486486486486486486486486486486L,
	0.14855469432313714290982231706729L, 0.86195286195286195286195286195286L,
	0.15191604202584197507180342489688L, 0.85906040268456375838926174496644L,
	0.15526612891112395152238330171010L, 0.85618729096989966555183946488294L,
	0.15860503017663858409337117462584L, 0.85333333333333333333333333333333L,
	0.16193282026931325324033828512361L, 0.85049833887043189368770764119601L,
	0.16524957289530716287561144927724L, 0.84768211920529801324503311258278L,
	0.16855536102980666694158653217010L, 0.84488448844884488448844884488449L,
	0.17185025692665922234009894605515L, 0.84210526315789473684210526315789L,
	0.17513433212784914801429146498639L, 0.83934426229508196721311475409836L,
	0.17840765747281829711940024151094L, 0.83660130718954248366013071895425L,
	0.18167030310763467826060559561708L, 0.83387622149837133550488599348534L,
	0.18492233849401199266390359265925L, 0.83116883116883116883116883116883L,
	0.18816383241818298682599058031055L, 0.82847896440129449838187702265372L,
	0.19139485299962945460929880756131L, 0.82580645161290322580645161290323L,
	0.19461546769967165885813859376727L, 0.82315112540192926045016077170418L,
	0.19782574332991988036257207119696L, 0.82051282051282051282051282051282L,
	0.20102574606059074134090833759180L, 0.81789137380191693290734824281150L,
	0.20421554142869089150382038619624L, 0.81528662420382165605095541401274L,
	0.20739519434607058715874557884901L, 0.81269841269841269841269841269841L,
	0.21056476910734963766955281273235L, 0.81012658227848101265822784810127L,
	0.21372432939771813886190519763320L, 0.80757097791798107255520504731861L,
	0.21687393830061435961908952574435L, 0.80503144654088050314465408805031L,
	0.22001365830528209590735863866163L, 0.80250783699059561128526645768025L,
	0.22314355131420975576629509030983L, 0.80000000000000000000000000000000L,
	0.22626367865045338936178708228039L, 0.79750778816199376947040498442368L,
	0.22937410106484582999148072504614L, 0.79503105590062111801242236024845L,
	0.23247487874309406492070507809557L, 0.79256965944272445820433436532508L,
	0.23556607131276690907758821894104L, 0.79012345679012345679012345679012L,
	0.23864773785017500991714913635228L, 0.78769230769230769230769230769231L,
	0.24171993688714516814430751591351L, 0.78527607361963190184049079754601L,
	0.24478272641769091643470471746631L, 0.78287461773700305810397553516820L,
	0.24783616390458125678060276574652L, 0.78048780487804878048780487804878L,
	0.25088030628580941658844644154994L, 0.77811550151975683890577507598784L,
	0.25391520998096344413732329790661L, 0.77575757575757575757575757575758L,
	0.25694093089750042544675986791122L, 0.77341389728096676737160120845921L,
	0.25995752443692606697207949454231L, 0.77108433734939759036144578313253L,
	0.26296504550088135182072917321109L, 0.76876876876876876876876876876877L,
	0.26596354849713794133912592653754L, 0.76646706586826347305389221556886L,
	0.26895308734550395893297435792450L, 0.76417910447761194029850746268657L,
	0.27193371548364175883166949453300L, 0.76190476190476190476190476190476L,
	0.27490548587279924916700958298302L, 0.75964391691394658753709198813056L,
	0.27786845100345630618635003292340L, 0.75739644970414201183431952662722L,
	0.28082266290088778463951975887313L, 0.75516224188790560471976401179941L,
	0.28376817313064459834690122235025L, 0.75294117647058823529411764705882L,
	0.28670503280395431465325093084207L, 0.75073313782991202346041055718475L,
	0.28963329258304267687889305552567L, 0.74853801169590643274853801169591L,
	0.29255300268637743997820125866413L, 0.74635568513119533527696793002915L,
	0.29546421289383587638668190605496L, 0.74418604651162790697674418604651L,
	0.29836697255179728146490043029350L, 0.74202898550724637681159420289855L,
	0.30126133057816178101287553823376L, 0.73988439306358381502890173410405L,
	0.30414733546729671701581987472045L, 0.73775216138328530259365994236311L,
	0.30702503529491186207512454053538L, 0.73563218390804597701149425287356L,
	0.30989447772286468786162455083323L, 0.73352435530085959885386819484241L,
	0.31275571000389688838624655968832L, 0.73142857142857142857142857142857L,
	0.31560877898630333490136618066748L, 0.72934472934472934472934472934473L,
	0.31845373111853461581024721359060L, 0.72727272727272727272727272727273L,
	0.32129061245373429205786314552256L, 0.72521246458923512747875354107649L,
	0.32411946865421197609067076043499L, 0.72316384180790960451977401129944L,
	0.32694034499585332059235689407381L, 0.72112676056338028169014084507042L,
	0.32975328637246798181442281192079L, 0.71910112359550561797752808988764L,
	0.33255833730007660141227562657342L, 0.71708683473389355742296918767507L,
	0.33535554192113783025717957981417L, 0.71508379888268156424581005586592L,
	0.33814494400871639771023591393927L, 0.71309192200557103064066852367688L,
	0.34092658697059321030508919978036L, 0.71111111111111111111111111111111L,
	0.34370051385331844468019789211029L, 0.70914127423822714681440443213296L,
	0.34646676734620858091846218842577L, 0.70718232044198895027624309392265L,
	0.34922538978528830418127542118737L, 0.70523415977961432506887052341598L,
	0.35197642315717818465544745625944L, 0.70329670329670329670329670329670L,
	0.35471990910292902835501121899932L, 0.70136986301369863013698630136986L,
	0.35745588892180377422600949014090L, 0.69945355191256830601092896174863L,
	0.36018440357500779628157496749302L, 0.69754768392370572207084468664850L,
	0.36290549368936845313782434597749L, 0.69565217391304347826086956521739L,
	0.36561919956096471131939687521705L, 0.69376693766937669376693766937669L,
	0.36832556115870765304823015405040L, 0.69189189189189189189189189189189L,
	0.37102461812787266391196491080682L, 0.69002695417789757412398921832884L,
	0.37371640979358408082101683271582L, 0.68817204301075268817204301075269L,
	0.37640097516425306599787763343625L, 0.68632707774798927613941018766756L,
	0.37907835293496945839085334563102L, 0.68449197860962566844919786096257L,
	0.38174858149084833985966626493568L, 0.68266666666666666666666666666667L,
	0.38441169891033203973479006248129L, 0.68085106382978723404255319148936L,
	0.38706774296844828789890250226182L, 0.67904509283819628647214854111406L,
	0.38971675114002521337046360400352L, 0.67724867724867724867724867724868L,
	0.39235876060286387247937961198822L, 0.67546174142480211081794195250660L,
	0.39499380824086897810639403636498L, 0.67368421052631578947368421052632L,
	0.39762193064713848910482907297341L, 0.67191601049868766404199475065617L,
	0.40024316412701270692932510199513L, 0.67015706806282722513089005235602L,
	0.40285754470108351465519756548706L, 0.66840731070496083550913838120104L,
	0.40546510810816438197801311546435L, 0.66666666666666666666666666666667L,
	0.40806588980822174843019868296908L, 0.66493506493506493506493506493506L,
	0.41065992498526838593430620317582L, 0.66321243523316062176165803108808L,
	0.41324724855021933092547601552549L, 0.66149870801033591731266149870801L,
	0.41582789514371096561332889295490L, 0.65979381443298969072164948453608L,
	0.41840189913888381751076326196676L, 0.65809768637532133676092544987147L,
	0.42096929464412963612886716150680L, 0.65641025641025641025641025641026L,
	0.42353011550580329571843047801791L, 0.65473145780051150895140664961637L,
	0.42608439531090006312454487959548L, 0.65306122448979591836734693877551L,
	0.42863216738969876020681227642664L, 0.65139949109414758269720101781170L,
	0.43117346481837134085917247895559L, 0.64974619289340101522842639593909L,
	0.43370832042155939343584790304219L, 0.64810126582278481012658227848101L,
	0.43623676677491807034904132306112L, 0.64646464646464646464646464646465L,
	0.43875883620762793774557505851145L, 0.64483627204030226700251889168766L,
	0.44127456080487522948949644166130L, 0.64321608040201005025125628140704L,
	0.44378397241030098117176844058815L, 0.64160401002506265664160401002506L,
	0.44628710262841951153259018061967L, 0.64000000000000000000000000000000L,
	0.44878398282700671051282211568394L, 0.63840399002493765586034912718204L,
	0.45127464413945858514469238307901L, 0.63681592039800995024875621890547L,
	0.45375911746712050664479479444226L, 0.63523573200992555831265508684864L,
	0.45623743348158759438080553816393L, 0.63366336633663366336633663366337L,
	0.45870962262697666484388330925088L, 0.63209876543209876543209876543210L,
	0.46117571512217016636799992559786L, 0.63054187192118226600985221674877L,
	0.46363574096303251309218227733116L, 0.62899262899262899262899262899263L,
	0.46608972992459922455861924750477L, 0.62745098039215686274509803921569L,
	0.46853771156323927037566523746297L, 0.62591687041564792176039119804401L,
	0.47097971521879101254689785605636L, 0.62439024390243902439024390243902L,
	0.47341577001667213137257839323698L, 0.62287104622871046228710462287105L,
	0.47584590486996391426520958630438L, 0.62135922330097087378640776699029L,
	0.47827014848147028038354614549746L, 0.61985472154963680387409200968523L,
	0.48068852934575190767661845544801L, 0.61835748792270531400966183574879L,
	0.48310107575113582273837458485215L, 0.61686746987951807228915662650602L,
	0.48550781578170080780179107719079L, 0.61538461538461538461538461538462L,
	0.48790877731923897324617318413266L, 0.61390887290167865707434052757794L,
	0.49030398804519383815034615964575L, 0.61244019138755980861244019138756L,
	0.49269347544257525569507695002008L, 0.61097852028639618138424821002387L,
	0.49507726679785151459796458484283L, 0.60952380952380952380952380952381L,
	0.49745538920281894225085925673168L, 0.60807600950118764845605700712589L,
	0.49982786955644932982133141524704L, 0.60663507109004739336492890995261L,
	0.50219473456671549427358417195181L, 0.60520094562647754137115839243499L,
	0.50455601075239528705830853173817L, 0.60377358490566037735849056603774L,
	0.50691172444485435411319631266009L, 0.60235294117647058823529411764706L,
	0.50926190178980794680407491922832L, 0.60093896713615023474178403755869L,
	0.51160656874906207851888487520338L, 0.59953161592505854800936768149883L,
	0.51394575110223431680100608827422L, 0.59813084112149532710280373831776L,
	0.51627947444845449617281928478756L, 0.59673659673659673659673659673660L,
	0.51860776420804563215297699636480L, 0.59534883720930232558139534883721L,
	0.52093064562418531240980983465964L, 0.59396751740139211136890951276102L,
	0.52324814376454783651680722493487L, 0.59259259259259259259259259259259L,
	0.52556028352292737138242760230713L, 0.59122401847575057736720554272517L,
	0.52786708962084238511389221777830L, 0.58986175115207373271889400921659L,
	0.53016858660912161784141963084521L, 0.58850574712643678160919540229885L,
	0.53246479886947184387392372346014L, 0.58715596330275229357798165137615L,
	0.53475575061602767547792329203264L, 0.58581235697940503432494279176201L,
	0.53704146589688365456672924415383L, 0.58447488584474885844748858447489L,
	0.53932196859560887465535515807734L, 0.58314350797266514806378132118451L,
	0.54159728243274437157654230390043L, 0.58181818181818181818181818181818L,
	0.54386743096728351766333898906600L, 0.58049886621315192743764172335601L,
	0.54613243759813565038239720923121L, 0.57918552036199095022624434389140L,
	0.54839232556557316274815028617986L, 0.57787810383747178329571106094808L,
	0.55064711795266227925994817920491L, 0.57657657657657657657657657657658L,
	0.55289683768667773758071790223062L, 0.57528089887640449438202247191011L,
	0.55514150754050159271548035951590L, 0.57399103139013452914798206278027L,
	0.55738115013400635704981654036123L, 0.57270693512304250559284116331096L,
	0.55961578793542268627088850052683L, 0.57142857142857142857142857142857L,
	0.56184544326269181791566481916070L, 0.57015590200445434298440979955457L,
	0.56407013828480296607138429009019L, 0.56888888888888888888888888888889L,
	0.56628989502311587259084997933712L, 0.56762749445676274944567627494457L,
	0.56850473535266871207873876486696L, 0.56637168141592920353982300884956L,
	0.57071468100347154485362456474159L, 0.56512141280353200883002207505519L,
	0.57291975356178550909275672662626L, 0.56387665198237885462555066079295L,
	0.57511997447138794042174254656927L, 0.56263736263736263736263736263736L,
	0.57731536503482360431811206151950L, 0.56140350877192982456140350877193L,
	0.57950594641464222385527440948807L, 0.56017505470459518599562363238512L,
	0.58169173963462248252061075372537L, 0.55895196506550218340611353711790L,
	0.58387276558098267909741335697529L, 0.55773420479302832244008714596950L,
	0.58604904500357820890411943628732L, 0.55652173913043478260869565217391L,
	0.58822059851708604303486822160911L, 0.55531453362255965292841648590022L,
	0.59038744660217637464191670812360L, 0.55411255411255411255411255411255L,
	0.59254960960667159874199020959330L, 0.55291576673866090712742980561555L,
	0.59470710774669278951434354652921L, 0.55172413793103448275862068965517L,
	0.59685996110779383658731192302566L, 0.55053763440860215053763440860215L,
	0.59900818964608339938160002446165L, 0.54935622317596566523605150214592L,
	0.60115181318933483619167431706886L, 0.54817987152034261241970021413276L,
	0.60329085143808426234058518666131L, 0.54700854700854700854700854700855L,
	0.60542532396671688943756776814149L, 0.54584221748400852878464818763326L,
	0.60755525022454179550108515279113L, 0.54468085106382978723404255319149L,
	0.60968064953685527348183350166059L, 0.54352441613588110403397027600849L,
	0.61180154110599290352988976642881L, 0.54237288135593220338983050847458L,
	0.61391794401237049219692911964556L, 0.54122621564482029598308668076110L,
	0.61602987721551401964756592819670L, 0.54008438818565400843881856540084L,
	0.61813735955507873387268912667482L, 0.53894736842105263157894736842105L,
	0.62024040975185752885149463256725L, 0.53781512605042016806722689075630L,
	0.62233904640877874159710264120870L, 0.53668763102725366876310272536688L,
	0.62443328801189350104253874405467L, 0.53556485355648535564853556485356L,
	0.62652315293135275977882085973420L, 0.53444676409185803757828810020877L,
	0.62860865942237413774430820577418L, 0.53333333333333333333333333333333L,
	0.63068982562619870508372614093135L, 0.53222453222453222453222453222453L,
	0.63276666957103782954578646850358L, 0.53112033195020746887966804979253L,
	0.63483920917301021196949384051049L, 0.53002070393374741200828157349896L,
	0.63690746223706923162049442718120L, 0.52892561983471074380165289256198L,
	0.63897144645792072137962398326474L, 0.52783505154639175257731958762887L,
	0.64103117942093129105560133440539L, 0.52674897119341563786008230452675L,
	0.64308667860302731539205385958513L, 0.52566735112936344969199178644764L,
	0.64513796137358470166522849613473L, 0.52459016393442622950819672131148L,
	0.64718504499530955012232063137786L, 0.52351738241308793456032719836401L,
	0.64922794662510981889083996990531L, 0.52244897959183673469387755102041L,
	0.65126668331495810339633335334967L, 0.52138492871690427698574338085540L,
	0.65330127201274563875861588121087L, 0.52032520325203252032520325203252L,
	0.65533172956312763209494967856963L, 0.51926977687626774847870182555781L,
	0.65735807270836003014189002324594L, 0.51821862348178137651821862348178L,
	0.65938031808912782611533641337096L, 0.51717171717171717171717171717172L,
	0.66139848224536500826023583870965L, 0.51612903225806451612903225806452L,
	0.66341258161706625109695030429080L, 0.51509054325955734406438631790744L,
	0.66542263254509044895009261000666L, 0.51405622489959839357429718875502L,
	0.66742865127195618994723416631898L, 0.51302605210420841683366733466934L,
	0.66943065394262926729888527092950L, 0.51200000000000000000000000000000L,
	0.67142865660530232331713904200189L, 0.51097804391217564870259481037924L,
	0.67342267521216672029796038880102L, 0.50996015936254980079681274900398L,
	0.67541272562017673108090414397020L, 0.50894632206759443339960238568588L,
	0.67739882359180614080968260999735L, 0.50793650793650793650793650793651L,
	0.67938098479579735014710062847376L, 0.50693069306930693069306930693069L,
	0.68135922480790306894807155956809L, 0.50592885375494071146245059288538L,
	0.68333355911162068816436314838775L, 0.50493096646942800788954635108481L,
	0.68530400309891941654404807896723L, 0.50393700787401574803149606299213L,
	0.68727057207096026749700688439435L, 0.50294695481335952848722986247544L,
	0.68923328123880898032491433781460L, 0.50196078431372549019607843137255L,
	0.69314718055994530941723212145818L, 0.50000000000000000000000000000000L
);

_Check_return_ float64 __cdecl ln(_In_ float64 _X, LOG_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::log(_X);
	#else

	if (_X <= 0) { return wrtval(NEG_INF_DOUBLE); }

	const int64 LN_MASK2 = (1i64 << (_Conf.BITSFRAC - _Conf.SCALE)) - 1;

	// LOG64F
	// Coefficients of talor series
	// A7 * x + A6 * x^2 + A5 * x^3 + A4 * x^4 + A3 * x^5 + A2 * x^6 + A1 * x^7 + A0 * x^8
	const float96 A7 = +1.00000000000000000000000000000000L;
	const float96 A6 = -0.50000000000000000000000000000000L;
	const float96 A5 = +0.33333333333333333333333333333333L;
	const float96 A4 = -0.25000000000000000000000000000000L;
	const float96 A3 = +0.20000000000000000000000000000000L;
	const float96 A2 = -0.16666666666666666666666666666667L;
	const float96 A1 = +0.14285714285714285714285714285714L;
	const float96 A0 = -0.12500000000000000000000000000000L;

	// Create buffer and load parameter
	int64 Buffer;
	int64 i0 = wrtval<float64, int64>(_X); // extract bytes of the number

	int64 Offset = (1 << (_Conf.BITSEXPO - 1)) - 1; // Calculate offset

	// Locate paremeters
	Buffer = (i0 & LN_MASK2) | (Offset << _Conf.BITSFRAC);
	int Index = int(i0 >> (_Conf.BITSFRAC - _Conf.SCALE - 1)) & (_Conf.MASK * 2);

	// ln(x) = log2(x) * ln2
	int Expo = (1 << _Conf.BITSEXPO) - 1;
	float64 y0 = ((int(i0 >> _Conf.BITSFRAC) & Expo) - Offset) * CSE_LN2 + _Conf.TABLE[Index];
	float64 x0 = (wrtval(Buffer) - 1.) * _Conf.TABLE[Index + 1] + (Index == _Conf.TABSIZE - 2 ? -1. / _Conf.TABSIZE : 0.);
	float64 xq = x0 * x0;

	// Sort the coefficients by +/- , and simplify the formula.
	// and get an equation contains positive terms and another equation contains negative terms.
	return y0
		+ (((A0 * xq + A2) * xq + A4) * xq + A6) * xq  // Negative
		+ (((A1 * xq + A3) * xq + A5) * xq + A7) * x0; // Positive

	#endif
}

// Use Logarithms change of base rule to derivate log10 and log with base.

_Check_return_ float64 __cdecl log(_In_ float64 _X, LOG_CONFIG _Conf) // Log10
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::log10(_X);
	#else
	return ln(_X, _Conf) / CSE_LN10;
	#endif
}

_Check_return_ float64 __cdecl log(_In_ float64 _Base, _In_ float64 _X, LOG_CONFIG _Conf)
{
	return ln(_X, _Conf) / ln(_Base, _Conf);
}

// Complex Form

_Check_return_ complex64 __cdecl ln(_In_ complex64 _X, LOG_CONFIG _Conf)
{
	// Convert to polar form (r, tet)
	float64 r = _CSE abs(_X);
	float64 tet = std::atan2(_X.imag(), _X.real());
	return ln(r) + 1i * float64(tet + 2. * CSE_PI * _Conf.K_OFFSET);
}

_Check_return_ complex64 __cdecl log(_In_ complex64 _X, LOG_CONFIG _Conf)
{
	return ln(_X, _Conf) / complex64(CSE_LN10);
}

_Check_return_ complex64 __cdecl log(_In_ complex64 _Base, _In_ complex64 _X, LOG_CONFIG _Conf)
{
	return ln(_X, _Conf) / ln(_Base, _Conf);
}

_CSE_END