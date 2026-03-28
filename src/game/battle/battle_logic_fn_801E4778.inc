    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r24, r3
    mr r31, r4
    bl fn_801ECA10
    cmpwi r3, 0x0
    bne @L_801E47A4
    li r3, 0x0
    b @L_801E4A58
@L_801E47A4:
    lwz r0, -21432(r13)
    cmpwi r0, 0x0
    bne @L_801E47B8
    li r3, 0x0
    b @L_801E4A58
@L_801E47B8:
    lis r3, -32697
    addi r30, r3, -21408
    lwz r0, 0xa0(r30)
    cmpwi r0, 0x0
    beq @L_801E47D4
    li r3, 0x0
    b @L_801E4A58
@L_801E47D4:
    addi r29, r30, 0x80
    li r4, 0x0
    mr r3, r29
    li r5, 0xc
    bl memset
    lis r3, -32697
    li r4, 0x0
    addi r3, r3, -21408
    li r5, 0x10
    addi r28, r3, 0x8c
    mr r3, r28
    bl memset
    lis r4, -32697
    mr r3, r24
    addi r4, r4, -21408
    bl fn_800A501C
    cmpwi r3, 0x0
    bne @L_801E4824
    li r3, 0x0
    b @L_801E4A58
@L_801E4824:
    lis r3, -32697
    lis r4, -32697
    addi r3, r3, -21408
    li r5, 0x40
    addi r4, r4, -23328
    li r6, 0x0
    li r7, 0x2
    bl fn_800A541C
    cmpwi r3, 0x0
    bge @L_801E4860
    lis r3, -32697
    addi r3, r3, -21408
    bl fn_800A50E4
    li r3, 0x0
    b @L_801E4A58
@L_801E4860:
    lis r4, -32697
    lis r3, -32697
    addi r4, r4, -21408
    li r5, 0x30
    addi r24, r4, 0x3c
    addi r4, r3, -23328
    mr r3, r24
    bl memcpy
    mr r3, r24
    addi r4, r2, -20980
    bl fn_800CA7FC
    cmpwi r3, 0x0
    beq @L_801E48A8
    lis r3, -32697
    addi r3, r3, -21408
    bl fn_800A50E4
    li r3, 0x0
    b @L_801E4A58
@L_801E48A8:
    lis r3, -32697
    addi r3, r3, -21408
    lwz r4, 0x40(r3)
    subis r0, r4, 0x1
    cmplwi r0, 0x1000
    beq @L_801E48CC
    bl fn_800A50E4
    li r3, 0x0
    b @L_801E4A58
@L_801E48CC:
    lwz r24, 0x5c(r3)
    lis r4, -32697
    addi r4, r4, -23328
    li r5, 0x20
    mr r6, r24
    li r7, 0x2
    bl fn_800A541C
    cmpwi r3, 0x0
    bge @L_801E4904
    lis r3, -32697
    addi r3, r3, -21408
    bl fn_800A50E4
    li r3, 0x0
    b @L_801E4A58
@L_801E4904:
    lis r4, -32697
    lis r3, -32697
    addi r4, r4, -21408
    li r5, 0x14
    addi r26, r4, 0x6c
    addi r4, r3, -23328
    mr r3, r26
    bl memcpy
    lis r3, -32697
    addi r25, r24, 0x14
    li r0, 0x0
    li r24, 0x0
    addi r27, r3, -21408
    stb r0, 0xa7(r27)
    b @L_801E4A24
@L_801E4940:
    add r3, r27, r24
    lbz r0, 0x70(r3)
    cmpwi r0, 0x1
    beq @L_801E49B8
    bge @L_801E4A18
    cmpwi r0, 0x0
    bge @L_801E4960
    b @L_801E4A18
@L_801E4960:
    lis r3, -32697
    lis r4, -32697
    addi r3, r3, -21408
    mr r6, r25
    addi r4, r4, -23328
    li r5, 0x20
    li r7, 0x2
    bl fn_800A541C
    cmpwi r3, 0x0
    bge @L_801E499C
    lis r3, -32697
    addi r3, r3, -21408
    bl fn_800A50E4
    li r3, 0x0
    b @L_801E4A58
@L_801E499C:
    lis r4, -32697
    mr r3, r29
    addi r4, r4, -23328
    li r5, 0xc
    bl memcpy
    addi r25, r25, 0xc
    b @L_801E4A20
@L_801E49B8:
    lis r3, -32697
    lis r4, -32697
    addi r3, r3, -21408
    mr r6, r25
    addi r4, r4, -23328
    li r5, 0x20
    li r7, 0x2
    bl fn_800A541C
    cmpwi r3, 0x0
    bge @L_801E49F4
    lis r3, -32697
    addi r3, r3, -21408
    bl fn_800A50E4
    li r3, 0x0
    b @L_801E4A58
@L_801E49F4:
    lis r4, -32697
    mr r3, r28
    addi r4, r4, -23328
    li r5, 0x10
    bl memcpy
    li r0, 0x1
    addi r25, r25, 0x10
    stb r0, 0xa7(r27)
    b @L_801E4A20
@L_801E4A18:
    li r3, 0x0
    b @L_801E4A58
@L_801E4A20:
    addi r24, r24, 0x1
@L_801E4A24:
    lwz r0, 0x0(r26)
    cmplw r24, r0
    blt @L_801E4940
    lis r3, -32697
    li r0, 0x1
    addi r4, r3, -21408
    li r5, 0x0
    stb r5, 0xa5(r4)
    li r3, 0x1
    stb r5, 0xa4(r4)
    stb r5, 0xa6(r4)
    stw r31, 0xb0(r4)
    stw r0, 0xa0(r30)
@L_801E4A58:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
