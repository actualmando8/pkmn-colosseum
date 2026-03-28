    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r31, r4
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065A90
    bge @L_80065AA4
    cmpwi r3, 0x0
    bge @L_80065A7C
    b @L_80065AA4
@L_80065A7C:
    li r30, 0x0
    li r29, 0x1
    li r28, 0x2
    li r27, 0x3
    b @L_80065AB4
@L_80065A90:
    li r30, 0x0
    li r28, 0x1
    li r29, 0x2
    li r27, 0x3
    b @L_80065AB4
@L_80065AA4:
    li r30, 0x0
    li r28, 0x1
    li r29, 0x2
    li r27, 0x3
@L_80065AB4:
    lha r3, 0x6(r31)
    subi r0, r3, 0xb3b
    cmplwi r0, 0xb5
    bgt @L_800676D8
    lis r3, jumptable_802EDB7C@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802EDB7C@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065B08
    bge @L_80065B08
    cmpwi r3, 0x0
    bge @L_80065AFC
    b @L_80065B08
@L_80065AFC:
    cmpwi r30, 0x2
    blt @L_80065B08
    li r27, 0x0
@L_80065B08:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r30, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x30
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065B74
    bge @L_80065B74
    cmpwi r3, 0x0
    bge @L_80065B68
    b @L_80065B74
@L_80065B68:
    cmpwi r30, 0x2
    blt @L_80065B74
    li r27, 0x0
@L_80065B74:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r30, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x3c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065BE0
    bge @L_80065BE0
    cmpwi r3, 0x0
    bge @L_80065BD4
    b @L_80065BE0
@L_80065BD4:
    cmpwi r30, 0x2
    blt @L_80065BE0
    li r27, 0x0
@L_80065BE0:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r30, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x48
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065C4C
    bge @L_80065C4C
    cmpwi r3, 0x0
    bge @L_80065C40
    b @L_80065C4C
@L_80065C40:
    cmpwi r30, 0x2
    blt @L_80065C4C
    li r27, 0x0
@L_80065C4C:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r30, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x54
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065CB8
    bge @L_80065CB8
    cmpwi r3, 0x0
    bge @L_80065CAC
    b @L_80065CB8
@L_80065CAC:
    cmpwi r30, 0x2
    blt @L_80065CB8
    li r27, 0x0
@L_80065CB8:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r30, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x60
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065D24
    bge @L_80065D24
    cmpwi r3, 0x0
    bge @L_80065D18
    b @L_80065D24
@L_80065D18:
    cmpwi r30, 0x2
    blt @L_80065D24
    li r27, 0x0
@L_80065D24:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r30, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x6c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065D90
    bge @L_80065D90
    cmpwi r3, 0x0
    bge @L_80065D84
    b @L_80065D90
@L_80065D84:
    cmpwi r28, 0x2
    blt @L_80065D90
    li r27, 0x0
@L_80065D90:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r28, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x30
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065DFC
    bge @L_80065DFC
    cmpwi r3, 0x0
    bge @L_80065DF0
    b @L_80065DFC
@L_80065DF0:
    cmpwi r28, 0x2
    blt @L_80065DFC
    li r27, 0x0
@L_80065DFC:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r28, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x3c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065E68
    bge @L_80065E68
    cmpwi r3, 0x0
    bge @L_80065E5C
    b @L_80065E68
@L_80065E5C:
    cmpwi r28, 0x2
    blt @L_80065E68
    li r27, 0x0
@L_80065E68:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r28, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x48
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065ED4
    bge @L_80065ED4
    cmpwi r3, 0x0
    bge @L_80065EC8
    b @L_80065ED4
@L_80065EC8:
    cmpwi r28, 0x2
    blt @L_80065ED4
    li r27, 0x0
@L_80065ED4:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r28, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x54
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065F40
    bge @L_80065F40
    cmpwi r3, 0x0
    bge @L_80065F34
    b @L_80065F40
@L_80065F34:
    cmpwi r28, 0x2
    blt @L_80065F40
    li r27, 0x0
@L_80065F40:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r28, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x60
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80065FAC
    bge @L_80065FAC
    cmpwi r3, 0x0
    bge @L_80065FA0
    b @L_80065FAC
@L_80065FA0:
    cmpwi r28, 0x2
    blt @L_80065FAC
    li r27, 0x0
@L_80065FAC:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r28, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x6c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066018
    bge @L_80066018
    cmpwi r3, 0x0
    bge @L_8006600C
    b @L_80066018
@L_8006600C:
    cmpwi r29, 0x2
    blt @L_80066018
    li r27, 0x0
@L_80066018:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r29, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x30
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066084
    bge @L_80066084
    cmpwi r3, 0x0
    bge @L_80066078
    b @L_80066084
@L_80066078:
    cmpwi r29, 0x2
    blt @L_80066084
    li r27, 0x0
@L_80066084:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r29, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x3c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800660F0
    bge @L_800660F0
    cmpwi r3, 0x0
    bge @L_800660E4
    b @L_800660F0
@L_800660E4:
    cmpwi r29, 0x2
    blt @L_800660F0
    li r27, 0x0
@L_800660F0:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r29, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x48
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_8006615C
    bge @L_8006615C
    cmpwi r3, 0x0
    bge @L_80066150
    b @L_8006615C
@L_80066150:
    cmpwi r29, 0x2
    blt @L_8006615C
    li r27, 0x0
@L_8006615C:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r29, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x54
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800661C8
    bge @L_800661C8
    cmpwi r3, 0x0
    bge @L_800661BC
    b @L_800661C8
@L_800661BC:
    cmpwi r29, 0x2
    blt @L_800661C8
    li r27, 0x0
@L_800661C8:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r29, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x60
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r27, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066234
    bge @L_80066234
    cmpwi r3, 0x0
    bge @L_80066228
    b @L_80066234
@L_80066228:
    cmpwi r29, 0x2
    blt @L_80066234
    li r27, 0x0
@L_80066234:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r29, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x6c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r28, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800662A0
    bge @L_800662A0
    cmpwi r3, 0x0
    bge @L_80066294
    b @L_800662A0
@L_80066294:
    cmpwi r27, 0x2
    blt @L_800662A0
    li r28, 0x0
@L_800662A0:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r27, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x30
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r28, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_8006630C
    bge @L_8006630C
    cmpwi r3, 0x0
    bge @L_80066300
    b @L_8006630C
@L_80066300:
    cmpwi r27, 0x2
    blt @L_8006630C
    li r28, 0x0
@L_8006630C:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r27, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x3c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r28, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066378
    bge @L_80066378
    cmpwi r3, 0x0
    bge @L_8006636C
    b @L_80066378
@L_8006636C:
    cmpwi r27, 0x2
    blt @L_80066378
    li r28, 0x0
@L_80066378:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r27, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x48
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r28, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800663E4
    bge @L_800663E4
    cmpwi r3, 0x0
    bge @L_800663D8
    b @L_800663E4
@L_800663D8:
    cmpwi r27, 0x2
    blt @L_800663E4
    li r28, 0x0
@L_800663E4:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r27, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x54
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r28, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066450
    bge @L_80066450
    cmpwi r3, 0x0
    bge @L_80066444
    b @L_80066450
@L_80066444:
    cmpwi r27, 0x2
    blt @L_80066450
    li r28, 0x0
@L_80066450:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r27, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x60
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    li r28, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800664BC
    bge @L_800664BC
    cmpwi r3, 0x0
    bge @L_800664B0
    b @L_800664BC
@L_800664B0:
    cmpwi r27, 0x2
    blt @L_800664BC
    li r28, 0x0
@L_800664BC:
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    mulli r4, r27, 0x48
    lis r3, lbl_803A9F08@ha
    addi r0, r3, lbl_803A9F08@l
    add r3, r0, r4
    addi r3, r3, 0x6c
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800676D8
    lhz r5, 0x2(r3)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    bl fn_80068DBC
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x0
    bl fn_80068BB0
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    bl fn_800689FC
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066568
    bge @L_80066568
    cmpwi r3, 0x0
    bge @L_8006655C
    b @L_80066568
@L_8006655C:
    cmpwi r30, 0x2
    blt @L_80066568
    li r26, 0x0
@L_80066568:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r30
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066594
    b @L_80066598
@L_80066594:
    mr r0, r26
@L_80066598:
    clrlwi r0, r0, 16
    cmpwi r0, 0x0
    ble @L_800665B8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800665B8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800665F8
    bge @L_800665F8
    cmpwi r3, 0x0
    bge @L_800665EC
    b @L_800665F8
@L_800665EC:
    cmpwi r30, 0x2
    blt @L_800665F8
    li r26, 0x0
@L_800665F8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r30
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066624
    b @L_80066628
@L_80066624:
    mr r0, r26
@L_80066628:
    clrlwi r0, r0, 16
    cmpwi r0, 0x1
    ble @L_80066648
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066648:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066688
    bge @L_80066688
    cmpwi r3, 0x0
    bge @L_8006667C
    b @L_80066688
@L_8006667C:
    cmpwi r30, 0x2
    blt @L_80066688
    li r26, 0x0
@L_80066688:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r30
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_800666B4
    b @L_800666B8
@L_800666B4:
    mr r0, r26
@L_800666B8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x2
    ble @L_800666D8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800666D8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066718
    bge @L_80066718
    cmpwi r3, 0x0
    bge @L_8006670C
    b @L_80066718
@L_8006670C:
    cmpwi r30, 0x2
    blt @L_80066718
    li r26, 0x0
@L_80066718:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r30
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066744
    b @L_80066748
@L_80066744:
    mr r0, r26
@L_80066748:
    clrlwi r0, r0, 16
    cmpwi r0, 0x3
    ble @L_80066768
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066768:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800667A8
    bge @L_800667A8
    cmpwi r3, 0x0
    bge @L_8006679C
    b @L_800667A8
@L_8006679C:
    cmpwi r30, 0x2
    blt @L_800667A8
    li r26, 0x0
@L_800667A8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r30
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_800667D4
    b @L_800667D8
@L_800667D4:
    mr r0, r26
@L_800667D8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x4
    ble @L_800667F8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800667F8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066838
    bge @L_80066838
    cmpwi r3, 0x0
    bge @L_8006682C
    b @L_80066838
@L_8006682C:
    cmpwi r30, 0x2
    blt @L_80066838
    li r26, 0x0
@L_80066838:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r30
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066864
    b @L_80066868
@L_80066864:
    mr r0, r26
@L_80066868:
    clrlwi r0, r0, 16
    cmpwi r0, 0x5
    ble @L_80066888
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066888:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x0
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x1
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x2
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x3
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x4
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x5
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    bl fn_80068DBC
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x0
    bl fn_80068BB0
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    bl fn_800689FC
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066998
    bge @L_80066998
    cmpwi r3, 0x0
    bge @L_8006698C
    b @L_80066998
@L_8006698C:
    cmpwi r28, 0x2
    blt @L_80066998
    li r26, 0x0
@L_80066998:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r28
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_800669C4
    b @L_800669C8
@L_800669C4:
    mr r0, r26
@L_800669C8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x0
    ble @L_800669E8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800669E8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066A28
    bge @L_80066A28
    cmpwi r3, 0x0
    bge @L_80066A1C
    b @L_80066A28
@L_80066A1C:
    cmpwi r28, 0x2
    blt @L_80066A28
    li r26, 0x0
@L_80066A28:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r28
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066A54
    b @L_80066A58
@L_80066A54:
    mr r0, r26
@L_80066A58:
    clrlwi r0, r0, 16
    cmpwi r0, 0x1
    ble @L_80066A78
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066A78:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066AB8
    bge @L_80066AB8
    cmpwi r3, 0x0
    bge @L_80066AAC
    b @L_80066AB8
@L_80066AAC:
    cmpwi r28, 0x2
    blt @L_80066AB8
    li r26, 0x0
@L_80066AB8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r28
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066AE4
    b @L_80066AE8
@L_80066AE4:
    mr r0, r26
@L_80066AE8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x2
    ble @L_80066B08
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066B08:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066B48
    bge @L_80066B48
    cmpwi r3, 0x0
    bge @L_80066B3C
    b @L_80066B48
@L_80066B3C:
    cmpwi r28, 0x2
    blt @L_80066B48
    li r26, 0x0
@L_80066B48:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r28
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066B74
    b @L_80066B78
@L_80066B74:
    mr r0, r26
@L_80066B78:
    clrlwi r0, r0, 16
    cmpwi r0, 0x3
    ble @L_80066B98
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066B98:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066BD8
    bge @L_80066BD8
    cmpwi r3, 0x0
    bge @L_80066BCC
    b @L_80066BD8
@L_80066BCC:
    cmpwi r28, 0x2
    blt @L_80066BD8
    li r26, 0x0
@L_80066BD8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r28
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066C04
    b @L_80066C08
@L_80066C04:
    mr r0, r26
@L_80066C08:
    clrlwi r0, r0, 16
    cmpwi r0, 0x4
    ble @L_80066C28
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066C28:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066C68
    bge @L_80066C68
    cmpwi r3, 0x0
    bge @L_80066C5C
    b @L_80066C68
@L_80066C5C:
    cmpwi r28, 0x2
    blt @L_80066C68
    li r26, 0x0
@L_80066C68:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r28
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066C94
    b @L_80066C98
@L_80066C94:
    mr r0, r26
@L_80066C98:
    clrlwi r0, r0, 16
    cmpwi r0, 0x5
    ble @L_80066CB8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066CB8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x0
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x1
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x2
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x3
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x4
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x5
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    bl fn_80068DBC
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x0
    bl fn_80068BB0
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    bl fn_800689FC
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066DC8
    bge @L_80066DC8
    cmpwi r3, 0x0
    bge @L_80066DBC
    b @L_80066DC8
@L_80066DBC:
    cmpwi r29, 0x2
    blt @L_80066DC8
    li r26, 0x0
@L_80066DC8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r29
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066DF4
    b @L_80066DF8
@L_80066DF4:
    mr r0, r26
@L_80066DF8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x0
    ble @L_80066E18
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066E18:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066E58
    bge @L_80066E58
    cmpwi r3, 0x0
    bge @L_80066E4C
    b @L_80066E58
@L_80066E4C:
    cmpwi r29, 0x2
    blt @L_80066E58
    li r26, 0x0
@L_80066E58:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r29
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066E84
    b @L_80066E88
@L_80066E84:
    mr r0, r26
@L_80066E88:
    clrlwi r0, r0, 16
    cmpwi r0, 0x1
    ble @L_80066EA8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066EA8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066EE8
    bge @L_80066EE8
    cmpwi r3, 0x0
    bge @L_80066EDC
    b @L_80066EE8
@L_80066EDC:
    cmpwi r29, 0x2
    blt @L_80066EE8
    li r26, 0x0
@L_80066EE8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r29
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066F14
    b @L_80066F18
@L_80066F14:
    mr r0, r26
@L_80066F18:
    clrlwi r0, r0, 16
    cmpwi r0, 0x2
    ble @L_80066F38
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066F38:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80066F78
    bge @L_80066F78
    cmpwi r3, 0x0
    bge @L_80066F6C
    b @L_80066F78
@L_80066F6C:
    cmpwi r29, 0x2
    blt @L_80066F78
    li r26, 0x0
@L_80066F78:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r29
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80066FA4
    b @L_80066FA8
@L_80066FA4:
    mr r0, r26
@L_80066FA8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x3
    ble @L_80066FC8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80066FC8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80067008
    bge @L_80067008
    cmpwi r3, 0x0
    bge @L_80066FFC
    b @L_80067008
@L_80066FFC:
    cmpwi r29, 0x2
    blt @L_80067008
    li r26, 0x0
@L_80067008:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r29
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80067034
    b @L_80067038
@L_80067034:
    mr r0, r26
@L_80067038:
    clrlwi r0, r0, 16
    cmpwi r0, 0x4
    ble @L_80067058
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80067058:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80067098
    bge @L_80067098
    cmpwi r3, 0x0
    bge @L_8006708C
    b @L_80067098
@L_8006708C:
    cmpwi r29, 0x2
    blt @L_80067098
    li r26, 0x0
@L_80067098:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r29
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_800670C4
    b @L_800670C8
@L_800670C4:
    mr r0, r26
@L_800670C8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x5
    ble @L_800670E8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800670E8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x0
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x1
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x2
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x3
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x4
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x5
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    bl fn_80068DBC
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x0
    bl fn_80068BB0
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    bl fn_800689FC
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800671F8
    bge @L_800671F8
    cmpwi r3, 0x0
    bge @L_800671EC
    b @L_800671F8
@L_800671EC:
    cmpwi r27, 0x2
    blt @L_800671F8
    li r26, 0x0
@L_800671F8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r27
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80067224
    b @L_80067228
@L_80067224:
    mr r0, r26
@L_80067228:
    clrlwi r0, r0, 16
    cmpwi r0, 0x0
    ble @L_80067248
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80067248:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80067288
    bge @L_80067288
    cmpwi r3, 0x0
    bge @L_8006727C
    b @L_80067288
@L_8006727C:
    cmpwi r27, 0x2
    blt @L_80067288
    li r26, 0x0
@L_80067288:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r27
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_800672B4
    b @L_800672B8
@L_800672B4:
    mr r0, r26
@L_800672B8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x1
    ble @L_800672D8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800672D8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80067318
    bge @L_80067318
    cmpwi r3, 0x0
    bge @L_8006730C
    b @L_80067318
@L_8006730C:
    cmpwi r27, 0x2
    blt @L_80067318
    li r26, 0x0
@L_80067318:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r27
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80067344
    b @L_80067348
@L_80067344:
    mr r0, r26
@L_80067348:
    clrlwi r0, r0, 16
    cmpwi r0, 0x2
    ble @L_80067368
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80067368:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800673A8
    bge @L_800673A8
    cmpwi r3, 0x0
    bge @L_8006739C
    b @L_800673A8
@L_8006739C:
    cmpwi r27, 0x2
    blt @L_800673A8
    li r26, 0x0
@L_800673A8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r27
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_800673D4
    b @L_800673D8
@L_800673D4:
    mr r0, r26
@L_800673D8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x3
    ble @L_800673F8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800673F8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80067438
    bge @L_80067438
    cmpwi r3, 0x0
    bge @L_8006742C
    b @L_80067438
@L_8006742C:
    cmpwi r27, 0x2
    blt @L_80067438
    li r26, 0x0
@L_80067438:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r27
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_80067464
    b @L_80067468
@L_80067464:
    mr r0, r26
@L_80067468:
    clrlwi r0, r0, 16
    cmpwi r0, 0x4
    ble @L_80067488
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80067488:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r26, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800674C8
    bge @L_800674C8
    cmpwi r3, 0x0
    bge @L_800674BC
    b @L_800674C8
@L_800674BC:
    cmpwi r27, 0x2
    blt @L_800674C8
    li r26, 0x0
@L_800674C8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800676D8
    bl fn_8006B1D4
    clrlwi r26, r3, 16
    mr r3, r27
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmplw r0, r26
    bge @L_800674F4
    b @L_800674F8
@L_800674F4:
    mr r0, r26
@L_800674F8:
    clrlwi r0, r0, 16
    cmpwi r0, 0x5
    ble @L_80067518
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80067518:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x0
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x1
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x2
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x3
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x4
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x5
    bl fn_80068794
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r30
    li r6, 0x0
    bl fn_800688C4
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r28
    li r6, 0x1
    bl fn_800688C4
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    li r6, 0x2
    bl fn_800688C4
    b @L_800676D8
    mr r3, r26
    mr r4, r31
    mr r5, r27
    li r6, 0x3
    bl fn_800688C4
    b @L_800676D8
    li r3, 0x0
    bl fn_8025D9F0
    clrlwi r0, r3, 16
    cmpwi r0, 0x0
    beq @L_80067634
    b @L_80067648
@L_80067634:
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80067648:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r3, 0x0
    bl fn_8025D9F0
    clrlwi r0, r3, 16
    cmpwi r0, 0x0
    beq @L_80067674
    b @L_80067688
@L_80067674:
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_80067688:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
    li r3, 0x0
    bl fn_8025D9F0
    clrlwi r0, r3, 16
    cmpwi r0, 0x0
    beq @L_800676B4
    b @L_800676C8
@L_800676B4:
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_800676D8
@L_800676C8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
@L_800676D8:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
