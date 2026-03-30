    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r27, r3
    mr r28, r4
    lis r3, lbl_80268940@ha
    li r25, 0x0
    addi r31, r3, lbl_80268940@l
@L_80076078:
    li r26, 0x0
@L_8007607C:
    mr r3, r27
    rlwinm r4, r26, 0, 16, 31
    bl fn_8012AC08
    mr r4, r25
    bl fn_80076398
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800760AC
    rlwinm r0, r25, 1, 0, 30
    addi r3, r31, 0xfc
    lhzx r3, r3, r0
    b @L_80076320
@L_800760AC:
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_8007607C
    addi r25, r25, 0x1
    cmplwi r25, 0x6
    blt @L_80076078
    cmplwi r28, 0x0
    bne @L_800760D4
    li r3, 0x0
    b @L_80076320
@L_800760D4:
    li r30, 0x0
@L_800760D8:
    li r29, 0x0
@L_800760DC:
    mr r3, r27
    rlwinm r4, r29, 0, 16, 31
    bl fn_8012AC08
    mr r25, r3
    li r26, 0x0
    cmplwi r25, 0x0
    beq @L_80076110
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076114
@L_80076110:
    li r26, 0x1
@L_80076114:
    cmpwi r26, 0x0
    beq @L_80076124
    li r0, 0x1
    b @L_800762AC
@L_80076124:
    cmpwi r30, 0x1
    beq @L_80076170
    bge @L_8007613C
    cmpwi r30, 0x0
    bge @L_80076148
    b @L_80076298
@L_8007613C:
    cmpwi r30, 0x3
    bge @L_80076298
    b @L_80076198
@L_80076148:
    mr r3, r25
    bl fn_8011F4A8
    rlwinm r5, r3, 0, 24, 31
    lha r0, 0x0(r28)
    srawi r4, r5, 31
    rlwinm r3, r0, 1, 31, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_800762AC
@L_80076170:
    mr r3, r25
    bl fn_8011F4A8
    lha r0, 0x2(r28)
    rlwinm r5, r3, 0, 24, 31
    rlwinm r3, r5, 1, 31, 31
    srawi r4, r0, 31
    subfc r0, r5, r0
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_800762AC
@L_80076198:
    mr r3, r25
    bl fn_8011F1A0
    mr r25, r3
    bl fn_8006B420
    rlwinm r0, r25, 0, 16, 31
    mr r26, r3
    cmpwi r0, 0xaf
    beq @L_800761D0
    bge @L_800761D8
    cmpwi r0, 0x0
    beq @L_800761C8
    b @L_800761D8
@L_800761C8:
    li r3, 0x1
    b @L_800761E0
@L_800761D0:
    li r3, 0x0
    b @L_800761E0
@L_800761D8:
    mr r3, r25
    bl fn_80142984
@L_800761E0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800761F4
    li r0, 0x0
    b @L_800762AC
@L_800761F4:
    lwz r0, 0x8(r26)
    cmpwi r0, 0x1
    beq @L_80076224
    bge @L_80076210
    cmpwi r0, 0x0
    bge @L_8007621C
    b @L_80076290
@L_80076210:
    cmpwi r0, 0x3
    bge @L_80076290
    b @L_80076238
@L_8007621C:
    li r0, 0x1
    b @L_800762AC
@L_80076224:
    rlwinm r0, r25, 0, 16, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_800762AC
@L_80076238:
    lis r3, lbl_802EE458@ha
    lwz r0, lbl_80478928(r13)
    addi r5, r3, lbl_802EE458@l
    li r4, 0x0
    rlwinm r3, r25, 0, 16, 31
    mtctr r0
    cmplwi r0, 0x0
    ble @L_80076288
@L_80076258:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8007627C
    addi r0, r4, 0x18
    lbzx r0, r26, r0
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_800762AC
@L_8007627C:
    addi r5, r5, 0x2
    addi r4, r4, 0x1
    bdnz @L_80076258
@L_80076288:
    li r0, 0x1
    b @L_800762AC
@L_80076290:
    li r0, 0x0
    b @L_800762AC
@L_80076298:
    addi r3, r31, 0x108
    addi r5, r31, 0x118
    li r4, 0xfb
    bl fn_80196E10
    li r0, 0x0
@L_800762AC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800762C8
    rlwinm r0, r30, 1, 0, 30
    la r3, lbl_8047C0D0(r2)
    lhzx r3, r3, r0
    b @L_80076320
@L_800762C8:
    addi r29, r29, 0x1
    cmpwi r29, 0x6
    blt @L_800760DC
    addi r30, r30, 0x1
    cmplwi r30, 0x3
    blt @L_800760D8
    li r25, 0x0
@L_800762E4:
    mr r3, r27
    mr r4, r28
    mr r5, r25
    bl fn_80076F2C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076310
    rlwinm r0, r25, 1, 0, 30
    la r3, lbl_8047C0D8(r2)
    lhzx r3, r3, r0
    b @L_80076320
@L_80076310:
    addi r25, r25, 0x1
    cmplwi r25, 0x4
    blt @L_800762E4
    li r3, 0x0
@L_80076320:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
