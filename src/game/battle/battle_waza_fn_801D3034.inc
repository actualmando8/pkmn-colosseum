    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -21556(r13)
    cmplw r3, r0
    bne @L_801D30AC
    lwz r3, -21552(r13)
    cmplwi r3, 0x0
    bne @L_801D3064
    li r3, 0x8
    bl fn_80177A44
    b @L_801D3088
@L_801D3064:
    lwz r0, 0x18(r3)
    cmplwi r0, 0x0
    beq @L_801D3080
    lwz r0, 0x20(r3)
    cmplwi r0, 0x0
    beq @L_801D3080
    bl fn_80176AE4
@L_801D3080:
    li r0, 0x0
    stw r0, -21552(r13)
@L_801D3088:
    li r3, 0x0
    bl fn_801765F4
    li r0, 0x0
    stw r0, -21556(r13)
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D30AC
    li r3, 0x2
    bl fn_80177A44
@L_801D30AC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
