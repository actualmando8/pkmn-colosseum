    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    bl fn_800576B4
    cmpwi r3, 0x4
    beq @L_80054FB4
    bge @L_80054F08
    cmpwi r3, 0x2
    beq @L_80054F88
    bge @L_8005509C
    cmpwi r3, 0x1
    bge @L_80054F18
    b @L_8005509C
    @L_80054F08:
    cmpwi r3, 0x6
    beq @L_80055034
    bge @L_8005509C
    b @L_8005501C
    @L_80054F18:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_8005509C
    lbz r0, 0x95(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80054F50
    cmpwi r0, 0x20
    bge @L_80054F50
    mulli r0, r0, 0xc
    lis r3, lbl_80267398@ha
    addi r3, r3, lbl_80267398@l
    add r3, r3, r0
    lwz r30, 0x8(r3)
    @L_80054F50:
    bl fn_80056A78
    mr r31, r3
    extsb r5, r30
    extsb r4, r31
    li r3, 0x0
    bl fn_80134EF0
    bl fn_800574FC
    extsb r4, r31
    extsb r5, r30
    li r3, 0x0
    bl fn_80134AF8
    li r3, 0x2
    bl fn_800576C4
    b @L_8005509C
    @L_80054F88:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_8005509C
    li r3, 0x3
    bl fn_800576C4
    lwz r0, lbl_8047A560(r13)
    cmpwi r0, 0x0
    beq @L_8005509C
    li r0, 0x1
    stb r0, 0x98(r31)
    b @L_8005509C
    @L_80054FB4:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_8005509C
    lbz r0, 0x95(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80054FEC
    cmpwi r0, 0x20
    bge @L_80054FEC
    mulli r0, r0, 0xc
    lis r3, lbl_80267398@ha
    addi r3, r3, lbl_80267398@l
    add r3, r3, r0
    lwz r30, 0x8(r3)
    @L_80054FEC:
    bl fn_80056A78
    mr r31, r3
    bl fn_800574E0
    mr r4, r3
    extsb r5, r31
    extsb r6, r30
    li r3, 0x0
    bl fn_80134E10
    bl fn_800574A8
    li r3, 0x5
    bl fn_800576C4
    b @L_8005509C
    @L_8005501C:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_8005509C
    li r3, 0x0
    bl fn_800576C4
    b @L_8005509C
    @L_80055034:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_8005509C
    lbz r0, 0x95(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_8005506C
    cmpwi r0, 0x20
    bge @L_8005506C
    mulli r0, r0, 0xc
    lis r3, lbl_80267398@ha
    addi r3, r3, lbl_80267398@l
    add r3, r3, r0
    lwz r30, 0x8(r3)
    @L_8005506C:
    bl fn_80056A78
    mr r31, r3
    bl fn_800574E0
    mr r4, r3
    extsb r5, r31
    extsb r6, r30
    li r3, 0x0
    bl fn_80134E10
    bl fn_800574A8
    bl fn_80057400
    li r3, 0x3
    bl fn_800576C4
    @L_8005509C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
